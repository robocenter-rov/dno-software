#include "SimpleCommunicator.h"
#include "DataReader.h"
#include "Hash.h"
#include "Debug.h"
#include "MathConstants.h"

enum SEND_BLOCK_IDS {
	SBI_STATE = 0,
	SBI_SENSOR_DATA = 1,
	SBI_RAW_SENSOR_DATA = 2,
	SBI_CALIBRATED_SENSOR_DATA = 3,
	SBI_BLUETOOTH_MSG_RECEIVE = 4,
	SBI_MOTORS_STATE_RECEIVE = 5,
	SBI_PID_STATE_RECEIVE = 6,
};

enum RECEIVE_BLOCK_IDS {
	RBI_STATE = 0,
	RBI_DEVICES_STATE = 1,
	RBI_MOTORS_STATE = 2,
	RBI_MOVEMENT = 3,
	RBI_CONFIG = 4,
};

SimpleCommunicator_t::SimpleCommunicator_t(ConnectionProvider_t* connection_provider) {
	_connection_provider = connection_provider;
	_last_i2c_scan_token = 0;
	_last_received_i2c_scan_token = 0;
	_config_hash = 0;
	_last_msg_receive_time = 0;
	_last_msg_send_time = 0;
	_send_frequency = 50;
	_receive_time_out = 1000;
	_last_received_msg_id = 0;
	_last_sended_msg_id = 0;
	_receive_packets_leak = 0;
	_connected = false;
	_all_state.send_calibrated_sensor_data = false;
	_all_state.send_raw_sensor_data = false;
	_all_state.send_motors_state = false;
	_all_state.send_pid_state = false;
}

int SimpleCommunicator_t::Begin() {
	return _connection_provider->Begin();
}

void SimpleCommunicator_t::Stop() {
	
}

bool SimpleCommunicator_t::Connected() const {
	return millis() - _last_msg_receive_time < _receive_time_out;
}

int SimpleCommunicator_t::Update() {
	unsigned int readed_bytes = 0;
	if (_connection_provider->Receive(readed_bytes)) {
		return 1;
	}

	if (readed_bytes) {
		if (!_connected) {
			_connected = true;
			_on_connect.callback(_on_connect.data);
		}

		DataReader_t dr(_connection_provider->Buffer(), readed_bytes);

#define READ(val) if (dr.Read(val) < 0) { Serial.println("Too short buffer"); return 0; }
#define READASFLOAT(val, min, max) if (dr.ReadInt8AsFloat(val, min, max) < 0) { Serial.println("Too short buffer"); return 0; }

		uint32_t msg_id;
		READ(msg_id);

		LOG("Receive msg: ");
		LOGLN(msg_id);

		if (msg_id > _last_received_msg_id) {
			_receive_packets_leak += msg_id - _last_received_msg_id - 1;
		}

		_last_received_msg_id = msg_id;

		while (dr.Available()) {
			uint8_t t;
			READ(t);
			auto block_id = static_cast<RECEIVE_BLOCK_IDS>(t);

			LOG("Receive block: ");
			LOGLN(block_id);

			switch (block_id) {
				case SBI_STATE:
					struct {
						State_t state;
						uint8_t last_i2c_scan;
						uint16_t send_frequency;
						uint16_t receive_timeout;
					} state;
					READ(state);
					_all_state = state.state;
					_last_received_i2c_scan_token = state.last_i2c_scan;
					_send_frequency = state.send_frequency;
					_receive_time_out = state.receive_timeout;
					_on_state_receive.callback(
						_on_state_receive.data, 
						state.state.flashlight_state, 
						state.state.read_bluetooth, 
						state.state.send_raw_sensor_data, 
						state.state.send_calibrated_sensor_data, 
						state.state.send_pid_state, 
						state.state.send_motors_state, 
						state.last_i2c_scan);
				break;
				case RBI_DEVICES_STATE: {
					float ArmPos;
					float HandPos;
					float M1;
					float M2;
					float Cam1Pos;
					float Cam2Pos;
					struct
					{
						bool cam1Local : 1;
						bool cam2Local : 1;
					} camCoordSystems;
					READASFLOAT(ArmPos, -1, 1);
					READASFLOAT(HandPos, -1, 1);
					READASFLOAT(M1, -PI / 2, PI / 2);
					READASFLOAT(M2, -PI / 2, PI / 2);
					READASFLOAT(Cam1Pos, -PI, PI);
					READASFLOAT(Cam2Pos, -PI, PI);
					READ(camCoordSystems);

					_on_devices_state_receive.callback(_on_devices_state_receive.data,
						ArmPos,
						HandPos,
						M1,
						M2,
						Cam1Pos,
						Cam2Pos,
						camCoordSystems.cam1Local,
						camCoordSystems.cam2Local
					);
				} break;
				case RBI_MOTORS_STATE: {
					float M1;
					float M2;
					float M3;
					float M4;
					float M5;
					float M6;
					float M7;
					float M8;
					READASFLOAT(M1, -1, 1);
					READASFLOAT(M2, -1, 1);
					READASFLOAT(M3, -1, 1);
					READASFLOAT(M4, -1, 1);
					READASFLOAT(M5, -1, 1);
					READASFLOAT(M6, -1, 1);
					READASFLOAT(M7, -1, 1);
					READASFLOAT(M8, -1, 1);
					_on_motors_state_receive.callback(_on_motors_state_receive.data,
						M1,
						M2,
						M3,
						M4,
						M5,
						M6,
						M7,
						M8
					);
				} break;
				case RBI_MOVEMENT: {
					#pragma pack(push, 1)
					struct {
						bool auto_yaw : 1;
						bool auto_pitch : 1;
						bool auto_roll : 1;
						bool auto_depth : 1;
					} control_type;
					#pragma pack(pop)

					float local_x;
					float local_y;
					float local_z; 
					float global_x;
					float global_y;
					float global_z;
					float yaw;
					float pitch;
					float roll;

					READ(control_type);
					READASFLOAT(local_x, -4, 4);
					READASFLOAT(local_y, -4, 4);
					READASFLOAT(local_z, -4, 4);
					READASFLOAT(global_x, -4, 4);
					READASFLOAT(global_y, -4, 4);
					READ(global_z);

					if (control_type.auto_yaw) {
						dr.ReadInt8AsFloat(yaw, -PI, PI);
					} else {
						dr.ReadInt8AsFloat(yaw, -4, 4);
					}

					if (control_type.auto_pitch) {
						dr.ReadInt8AsFloat(pitch, -PI, PI);
					} else {
						dr.ReadInt8AsFloat(pitch, -4, 4);
					}

					if (control_type.auto_roll) {
						dr.ReadInt8AsFloat(roll, -PI, PI);
					} else {
						dr.ReadInt8AsFloat(roll, -4, 4);
					}

					_on_movement_receive.callback(_on_movement_receive.data,
						control_type.auto_yaw,
						control_type.auto_pitch,
						control_type.auto_roll,
						control_type.auto_depth,
						local_x,
						local_y,
						local_z,
						global_x,
						global_y,
						global_z,
						yaw,
						pitch,
						roll
					);
				} break;
				case RBI_CONFIG: {
					#pragma pack(push, 1)
					struct {
						struct {
							float depth_p;
							float depth_i;
							float depth_d;

							float yaw_p;
							float yaw_i;
							float yaw_d;

							float pitch_p;
							float pitch_i;
							float pitch_d;

							float roll_p;
							float roll_i;
							float roll_d;
						} pids;
						struct {
							uint8_t M1Pos;
							uint8_t M2Pos;
							uint8_t M3Pos;
							uint8_t M4Pos;
							uint8_t M5Pos;
							uint8_t M6Pos;
							uint8_t M7Pos;
							uint8_t M8Pos;
						} MPositions;
						struct {
							float M1mul;
							float M2mul;
							float M3mul;
							float M4mul;
							float M5mul;
							float M6mul;
							float M7mul;
							float M8mul;
						} MMultipliers;
						struct {
							float camera1_maxVal;
							float camera1_minVal;
							float camera2_maxVal;
							float camera2_minVal;
						} CamerasConfig;
						struct {
							float acc_x_bias;
							float acc_y_bias;
							float acc_z_bias;

							float acc_x_scale;
							float acc_y_scale;
							float acc_z_scale;

							float gyro_x_bias;
							float gyro_y_bias;
							float gyro_z_bias;
							
							float gyro_scale;
						} IMUConfig;
					} config;
#pragma pack(pop)
					READ(config);

					uint32_t config_hash;
					config_hash = HashLy(config);

					//Serial.println("Received config");

					if (_config_hash != config_hash)
					{
						Serial.println("Received new config");
						_on_pid_receive.callback(_on_pid_receive.data,
							config.pids.depth_p,
							config.pids.depth_i,
							config.pids.depth_d,

							config.pids.yaw_p,
							config.pids.yaw_i,
							config.pids.yaw_d,

							config.pids.pitch_p,
							config.pids.pitch_i,
							config.pids.pitch_d,

							config.pids.roll_p,
							config.pids.roll_i,
							config.pids.roll_d
						);

						_on_motors_config_receive.callback(_on_motors_config_receive.data,
							config.MPositions.M1Pos,
							config.MPositions.M2Pos,
							config.MPositions.M3Pos,
							config.MPositions.M4Pos,
							config.MPositions.M5Pos,
							config.MPositions.M6Pos,
							config.MPositions.M7Pos,
							config.MPositions.M8Pos,

							config.MMultipliers.M1mul,
							config.MMultipliers.M2mul,
							config.MMultipliers.M3mul,
							config.MMultipliers.M4mul,
							config.MMultipliers.M5mul,
							config.MMultipliers.M6mul,
							config.MMultipliers.M7mul,
							config.MMultipliers.M8mul
						);

						_on_cameras_config_receive.callback(_on_cameras_config_receive.data,
							config.CamerasConfig.camera1_maxVal, 
							config.CamerasConfig.camera1_minVal,
							config.CamerasConfig.camera2_maxVal,
							config.CamerasConfig.camera2_minVal
						);

						_on_imu_config_receive.callback(_on_imu_config_receive.data,
							config.IMUConfig.acc_x_bias,
							config.IMUConfig.acc_y_bias,
							config.IMUConfig.acc_z_bias,

							config.IMUConfig.acc_x_scale,
							config.IMUConfig.acc_y_scale,
							config.IMUConfig.acc_z_scale,

							config.IMUConfig.gyro_x_bias,
							config.IMUConfig.gyro_y_bias,
							config.IMUConfig.gyro_z_bias,

							config.IMUConfig.gyro_scale
						);

						_config_hash = config_hash;
					}
					
					} break;
				default: ;
			}
		}

		_last_msg_receive_time = millis();
	}

	unsigned long now = millis();
	
	if (now - _last_msg_receive_time >= _receive_time_out && _connected) {
		_on_disconnect.callback(_on_disconnect.data);
	}

	if (now - _last_msg_send_time > _send_frequency) {
		_connection_provider->BeginPacket();
		_connection_provider->Write(++_last_sended_msg_id);
		_connection_provider->Write(_receive_packets_leak);

		_connection_provider->Write(static_cast<uint8_t>(SBI_STATE));

		bool flashlight_state;
		_on_state_need.callback(_on_state_need.data, flashlight_state);
		struct {
			bool flashlight_state : 1;
		} state {flashlight_state};

		_connection_provider->Write(state);

		if (_last_i2c_scan_token != _last_received_i2c_scan_token) {
			bool scanned; bool pca1; bool pca2; bool pca3; bool hmc58x3; bool itg3200; bool adxl345; bool bmp085; bool ms5803;
			_on_scanned_i2c_devices_need.callback(_on_scanned_i2c_devices_need.data,
				scanned, pca1, pca2, pca3, hmc58x3, itg3200, adxl345, bmp085, ms5803);

			if (scanned) {
				_last_i2c_scan_token = _last_received_i2c_scan_token;
				_last_scanned_i2c_devices = { pca1, pca2, pca3, hmc58x3, itg3200, adxl345, bmp085, ms5803 };
			}
		}

		_connection_provider->Write(_last_i2c_scan_token);
		_connection_provider->Write(_last_scanned_i2c_devices);
		_connection_provider->Write(_config_hash);
		_connection_provider->Write<uint16_t>(now - _last_update_time);

		_connection_provider->Write<uint8_t>(SBI_SENSOR_DATA);

		struct {
			float q1;
			float q2;
			float q3;
			float q4;
			float depth;
		} sensor_data;
		_on_sensor_data_need.callback(_on_sensor_data_need.data,
			sensor_data.q1, sensor_data.q2, sensor_data.q3, sensor_data.q4, sensor_data.depth
		);

		_connection_provider->Write(sensor_data);

		if (_all_state.send_raw_sensor_data) {
			_connection_provider->Write(static_cast<uint8_t>(SBI_RAW_SENSOR_DATA));

			struct {
				int ax;
				int ay;
				int az;

				int mx;
				int my;
				int mz;

				int gx;
				int gy;
				int gz;

				float depth;
			} raw_sensor_data;

			_on_raw_sensor_data_need.callback(_on_raw_sensor_data_need.data,
				raw_sensor_data.ax, raw_sensor_data.ay, raw_sensor_data.az,
				raw_sensor_data.mx, raw_sensor_data.my, raw_sensor_data.mz,
				raw_sensor_data.gx, raw_sensor_data.gy, raw_sensor_data.gz,
				raw_sensor_data.depth
			);

			_connection_provider->Write(raw_sensor_data);
		}

		if (_all_state.send_calibrated_sensor_data) {
			_connection_provider->Write(static_cast<uint8_t>(SBI_CALIBRATED_SENSOR_DATA));

			struct {
				float ax;
				float ay;
				float az;

				float mx;
				float my;
				float mz;

				float gx;
				float gy;
				float gz;

				float depth;
			} calibrated_sensor_data;

			_on_calibrated_sensor_data_need.callback(_on_calibrated_sensor_data_need.data,
				calibrated_sensor_data.ax, calibrated_sensor_data.ay, calibrated_sensor_data.az,
				calibrated_sensor_data.mx, calibrated_sensor_data.my, calibrated_sensor_data.mz,
				calibrated_sensor_data.gx, calibrated_sensor_data.gy, calibrated_sensor_data.gz,
				calibrated_sensor_data.depth
			);

			_connection_provider->Write(calibrated_sensor_data);
		}

		if (_all_state.send_pid_state) {
			_connection_provider->Write(static_cast<uint8_t>(SBI_PID_STATE_RECEIVE));

			struct {
				float depth_in;
				float depth_target;
				float depth_out;

				float yaw_in;
				float yaw_target;
				float yaw_out;

				float pitch_in;
				float pitch_target;
				float pitch_out;

				float roll_in;
				float roll_target;
				float roll_out;
			} pids_state;

			_on_pids_state_need.callback(_on_pids_state_need.data,
				pids_state.depth_in, pids_state.depth_target, pids_state.depth_out,
				pids_state.yaw_in, pids_state.yaw_target, pids_state.yaw_out,
				pids_state.pitch_in, pids_state.pitch_target, pids_state.pitch_out,
				pids_state.roll_in, pids_state.roll_target, pids_state.roll_out
			);

			_connection_provider->Write(pids_state);
		}

		if (_all_state.send_motors_state) {
			_connection_provider->Write(static_cast<uint8_t>(SBI_MOTORS_STATE_RECEIVE));

			struct {
				float m1;
				float m2;
				float m3;
				float m4;
				float m5;
				float m6;
				float m7;
				float m8;
			} motors_state;

			_on_motors_state_need.callback(_on_motors_state_need.data,
				motors_state.m1, motors_state.m2, motors_state.m3, motors_state.m4, motors_state.m5, motors_state.m6, motors_state.m7, motors_state.m8
			);

			_connection_provider->Write(motors_state);
		}

		if (_all_state.read_bluetooth) {
			char bluetooth_message[20];
			bool readed;
			_on_bluetooth_msg_need.callback(_on_bluetooth_msg_need.data, bluetooth_message, readed);

			if (readed) {
				_connection_provider->Write(static_cast<uint8_t>(SBI_BLUETOOTH_MSG_RECEIVE));
				_connection_provider->Write(bluetooth_message, 7);
			}
		}

		_connection_provider->EndPacket();

		_last_msg_send_time = millis();
	}

	_last_update_time = millis();

	return 0;
}

void SimpleCommunicator_t::OnStateNeed(void(* callback)(void* data, bool& flashlight_state), void* data) {
	_on_state_need.callback = callback;
	_on_state_need.data = data;
}

void SimpleCommunicator_t::OnSensorDataNeed(void(* callback)(void* data, float& q1, float& q2, float& q3, float& q4, float& depth), void* data) {
	_on_sensor_data_need.callback = callback;
	_on_sensor_data_need.data = data;
}

void SimpleCommunicator_t::OnRawSensorDataNeed(void(* callback)(void* data, int& ax, int& ay, int& az, int& gx, int& gy, int& gz, int& mx, int& my, int& mz, float& depth), void* data) {
	_on_raw_sensor_data_need.callback = callback;
	_on_raw_sensor_data_need.data = data;
}

void SimpleCommunicator_t::OnCalibratedSensorDataNeed(void(* callback)(void* data, float& ax, float& ay, float& az, float& gx, float& gy, float& gz, float& mx, float& my, float& mz, float& depth), void* data) {
	_on_calibrated_sensor_data_need.callback = callback;
	_on_calibrated_sensor_data_need.data = data;
}

void SimpleCommunicator_t::OnBluetoothMessageNeed(void(* callback)(void* data, char* bluetooth_message, bool& readed), void* data) {
	_on_bluetooth_msg_need.callback = callback;
	_on_bluetooth_msg_need.data = data;
}

void SimpleCommunicator_t::OnPidsStateNeed(void(* callback)(void* data, float& depth_in, float& depth_target, float& depth_out, float& yaw_in, float& yaw_target, float& yaw_out, float& pitch_in, float& pitch_out, float& pitch_target, float& roll_in, float& roll_out, float& roll_target), void* data) {
	_on_pids_state_need.callback = callback;
	_on_pids_state_need.data = data;
}

void SimpleCommunicator_t::OnMotorsStateNeed(void(* callback)(void* data, float& m1, float& m2, float& m3, float& m4, float& m5, float& m6, float& m7, float& m8), void* data) {
	_on_motors_state_need.callback = callback;
	_on_motors_state_need.data = data;
}

void SimpleCommunicator_t::OnScannedI2CDevicesNeed(void(* callback)(void* data, bool& scanned, bool& pca1, bool& pca2, bool& pca3, bool& hmc58x3, bool& itg3200, bool& adxl345, bool& bmp085, bool& ms5803), void* data) {
	_on_scanned_i2c_devices_need.callback = callback;
	_on_scanned_i2c_devices_need.data = data;
}

void SimpleCommunicator_t::OnConnect(void(* callback)(void* data), void* data) {
	_on_connect.callback = callback;
	_on_connect.data = data;
}

void SimpleCommunicator_t::OnDisconnect(void(* callback)(void* data), void* data) {
	_on_disconnect.callback = callback;
	_on_disconnect.data = data;
}

void SimpleCommunicator_t::OnStateReceive(void(* callback)(void* data, bool flashlight_state, bool read_bluetooth, bool send_raw_sensor_data, bool send_calibrated_sensor_data, bool send_pid_state, bool send_motors_state, uint8_t i2c_scan_token), void* data) {
	_on_state_receive.callback = callback;
	_on_state_receive.data = data;
}

void SimpleCommunicator_t::OnDevicesStateReceive(void(* callback)(void* data, float arm_pos, float hand_pos, float m1_pos, float m2_pos, float cam1_pos, float cam2_pos, bool cam1local, bool cam2local), void* data) {
	_on_devices_state_receive.callback = callback;
	_on_devices_state_receive.data = data;
}

void SimpleCommunicator_t::OnMotorsStateReceive(void(* callback)(void* data, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8), void* data) {
	_on_motors_state_receive.callback = callback;
	_on_motors_state_receive.data = data;
}

void SimpleCommunicator_t::OnMotorsConfigReceive(void(* callback)(void* data, int m1pos, int m2pos, int m3pos, int m4pos, int m5pos, int m6pos, int m7pos, int m8pos, float m1mul, float m2mul, float m3mul, float m4mul, float m5mul, float m6mul, float m7mul, float m8mul), void* data) {
	_on_motors_config_receive.callback = callback;
	_on_motors_config_receive.data = data;
}

void SimpleCommunicator_t::OnMovementReceive(void(* callback)(void* data, bool auto_yaw, bool auto_pitch, bool auto_roll, bool auto_depth, float local_x, float local_y, float local_z, float global_x, float global_y, float global_z, float yaw, float pitch, float roll), void* data) {
	_on_movement_receive.callback = callback;
	_on_movement_receive.data = data;
}

void SimpleCommunicator_t::OnPidReceive(void(* callback)(void* data, float depth_p, float depth_i, float depth_d, float yaw_p, float yaw_i, float yaw_d, float pitch_p, float pitch_i, float pitch_d, float roll_p, float roll_i, float roll_d), void* data) {
	_on_pid_receive.callback = callback;
	_on_pid_receive.data = data;
}

void SimpleCommunicator_t::OnCamerasConfigReceive(void (* callback)(void* data, float camera1_maxVal, float camera1_minVal, float camera2_maxVal, float camera2_minVal), void* data)
{
	_on_cameras_config_receive.callback = callback;
	_on_cameras_config_receive.data = data;
}

void SimpleCommunicator_t::OnIMUConfigReceive(void(* callback)(void* data, float acc_x_bias, float acc_y_bias, float acc_z_bias, float acc_x_scale, float acc_y_scale, float acc_z_scale, float gyro_x_bias, float gyro_y_bias, float gyro_z_bias, float gyro_scale), void* data) {
	_on_imu_config_receive.callback = callback;
	_on_imu_config_receive.data = data;
}


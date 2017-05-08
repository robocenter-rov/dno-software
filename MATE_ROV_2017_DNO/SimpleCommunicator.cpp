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
	RBI_PID = 4,
	RBI_MOTORS_CONFIG = 5,
};

SimpleCommunicator_t::SimpleCommunicator_t(ConnectionProvider_t* connection_provider) {
	_connection_provider = connection_provider;
	_last_i2c_scan_token = 0;
	_last_received_i2c_scan_token = 0;
	_pids_hash = 0;
	_motors_config_hash = 0;
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


#define READ(val) if (dr.Read(val) < 0) { LOGLN("Too short buffer"); return 0; }

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
					dr.ReadInt8AsFloat(ArmPos, -1, 1);
					dr.ReadInt8AsFloat(HandPos, -1, 1);
					dr.ReadInt8AsFloat(M1, 0, PI2);
					dr.ReadInt8AsFloat(M2, 0, PI2);
					dr.ReadInt8AsFloat(Cam1Pos, 0, PI2);
					dr.ReadInt8AsFloat(Cam2Pos, 0, PI2);
					_on_devices_state_receive.callback(_on_devices_state_receive.data,
						ArmPos,
						HandPos,
						M1,
						M2,
						Cam1Pos,
						Cam2Pos
					);
				} break;
				case RBI_MOTORS_STATE: {
					float M1;
					float M2;
					float M3;
					float M4;
					float M5;
					float M6;
					dr.ReadInt8AsFloat(M1, -1, 1);
					dr.ReadInt8AsFloat(M2, -1, 1);
					dr.ReadInt8AsFloat(M3, -1, 1);
					dr.ReadInt8AsFloat(M4, -1, 1);
					dr.ReadInt8AsFloat(M5, -1, 1);
					dr.ReadInt8AsFloat(M6, -1, 1);
					_on_motors_state_receive.callback(_on_motors_state_receive.data,
						M1,
						M2,
						M3,
						M4,
						M5,
						M6
					);
				} break;
				case RBI_MOVEMENT: {
					#pragma pack(1)
					struct {
						bool auto_depth : 1;
						bool auto_yaw : 1;
						bool auto_pitch : 1;
					} control_type;
					#pragma pack(pop)

					float x;
					float y;
					float depth;
					float yaw;
					float pitch;

					dr.Read(control_type);
					dr.ReadInt8AsFloat(x, -2, 2);
					dr.ReadInt8AsFloat(y, -2, 2);
					dr.Read(depth);

					if (control_type.auto_yaw) {
						dr.ReadInt8AsFloat(yaw, -PI, PI);
					} else {
						dr.ReadInt8AsFloat(yaw, -1, 1);
					}

					if (control_type.auto_pitch) {
						dr.ReadInt8AsFloat(pitch, -PI, PI);
					} else {
						dr.ReadInt8AsFloat(pitch, -1, 1);
					}

					_on_movement_receive.callback(_on_movement_receive.data,
						control_type.auto_depth,
						control_type.auto_yaw,
						control_type.auto_pitch,
						x,
						y,
						depth,
						yaw,
						pitch
					);
				} break;
				case RBI_PID:
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
					} pids;
					READ(pids);

					uint32_t pids_hash;
					pids_hash = HashLy(pids);
					if (_pids_hash != pids_hash) {
						_on_pid_receive.callback(_on_pid_receive.data,
							pids.depth_p,
							pids.depth_i,
							pids.depth_d,

							pids.yaw_p,
							pids.yaw_i,
							pids.yaw_d,

							pids.pitch_p,
							pids.pitch_i,
							pids.pitch_d
						);
					}

					_pids_hash = pids_hash;
				break;
				case RBI_MOTORS_CONFIG: {
					#pragma pack(1)
					struct {
						struct {
							uint8_t M1Pos : 3;
							uint8_t M2Pos : 3;
							uint8_t M3Pos : 3;
							uint8_t M4Pos : 3;
							uint8_t M5Pos : 3;
							uint8_t M6Pos : 3;
						} MPositions;
						struct {
							float M1mul;
							float M2mul;
							float M3mul;
							float M4mul;
							float M5mul;
							float M6mul;
						} MMultipliers;
					} motors_config;
					#pragma pack(pop)
					READ(motors_config);

					uint32_t motors_config_hash;
					motors_config_hash = HashLy(motors_config);

					if (_motors_config_hash != motors_config_hash) {
						_on_motors_config_receive.callback(_on_motors_config_receive.data,
							motors_config.MPositions.M1Pos,
							motors_config.MPositions.M2Pos,
							motors_config.MPositions.M3Pos,
							motors_config.MPositions.M4Pos,
							motors_config.MPositions.M5Pos,
							motors_config.MPositions.M6Pos,

							motors_config.MMultipliers.M1mul,
							motors_config.MMultipliers.M2mul,
							motors_config.MMultipliers.M3mul,
							motors_config.MMultipliers.M4mul,
							motors_config.MMultipliers.M5mul,
							motors_config.MMultipliers.M6mul
						);

						_motors_config_hash = motors_config_hash;
					}
				}
				default: ;
			}
		}

		_last_msg_receive_time = millis();
	}

	unsigned long now = millis();
	
	if (now - _last_msg_receive_time >= _receive_time_out && _connected) {
		_on_disconnect.callback(_on_disconnect.data);
	}

	if (now - _last_msg_send_time > _send_frequency && now - _last_msg_receive_time < _receive_time_out) {
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
			bool scanned; bool pca1; bool pca2; bool hmc58x3; bool itg3200; bool adxl345; bool bmp085; bool ms5803;
			_on_scanned_i2c_devices_need.callback(_on_scanned_i2c_devices_need.data,
				scanned, pca1, pca2, hmc58x3, itg3200, adxl345, bmp085, ms5803);

			if (scanned) {
				_last_i2c_scan_token = _last_received_i2c_scan_token;
				_last_scanned_i2c_devices = { pca1, pca2, hmc58x3, itg3200, adxl345, bmp085, ms5803 };
			}
		}

		_connection_provider->Write(_last_i2c_scan_token);
		_connection_provider->Write(_last_scanned_i2c_devices);
		_connection_provider->Write(_pids_hash);
		_connection_provider->Write(_motors_config_hash);

		_connection_provider->Write(static_cast<uint8_t>(SBI_SENSOR_DATA));

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
			} pids_state;

			_on_pids_state_need.callback(_on_pids_state_need.data,
				pids_state.depth_in, pids_state.depth_target, pids_state.depth_out,
				pids_state.yaw_in, pids_state.yaw_target, pids_state.yaw_out,
				pids_state.pitch_in, pids_state.pitch_target, pids_state.pitch_out
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
			} motors_state;

			_on_motors_state_need.callback(_on_motors_state_need.data,
				motors_state.m1, motors_state.m2, motors_state.m3, motors_state.m4, motors_state.m5, motors_state.m6
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

void SimpleCommunicator_t::OnPidsStateNeed(void(* callback)(void* data, float& depth_in, float& depth_target, float& depth_out, float& yaw_in, float& yaw_target, float& yaw_out, float& pitch_in, float& pitch_out, float& pitch_target), void* data) {
	_on_pids_state_need.callback = callback;
	_on_pids_state_need.data = data;
}

void SimpleCommunicator_t::OnMotorsStateNeed(void(* callback)(void* data, float& m1, float& m2, float& m3, float& m4, float& m5, float& m6), void* data) {
	_on_motors_state_need.callback = callback;
	_on_motors_state_need.data = data;
}

void SimpleCommunicator_t::OnScannedI2CDevicesNeed(void(* callback)(void* data, bool& scanned, bool& pca1, bool& pca2, bool& hmc58x3, bool& itg3200, bool& adxl345, bool& bmp085, bool& ms5803), void* data) {
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

void SimpleCommunicator_t::OnDevicesStateReceive(void(* callback)(void* data, float arm_pos, float hand_pos, float m1_pos, float m2_pos, float cam1_pos, float cam2_pos), void* data) {
	_on_devices_state_receive.callback = callback;
	_on_devices_state_receive.data = data;
}

void SimpleCommunicator_t::OnMotorsStateReceive(void(* callback)(void* data, float m1, float m2, float m3, float m4, float m5, float m6), void* data) {
	_on_motors_state_receive.callback = callback;
	_on_motors_state_receive.data = data;
}

void SimpleCommunicator_t::OnMotorsConfigReceive(void(* callback)(void* data, int m1pos, int m2pos, int m3pos, int m4pos, int m5pos, int m6pos, float m1mul, float m2mul, float m3mul, float m4mul, float m5mul, float m6mul), void* data) {
	_on_motors_config_receive.callback = callback;
	_on_motors_config_receive.data = data;
}

void SimpleCommunicator_t::OnMovementReceive(void(* callback)(void* data, bool auto_depth, bool auto_yaw, bool auto_pitch, float x, float y, float depth, float yaw, float pitch), void* data) {
	_on_movement_receive.callback = callback;
	_on_movement_receive.data = data;
}

void SimpleCommunicator_t::OnPidReceive(void(* callback)(void* data, float depth_p, float depth_i, float depth_d, float yaw_p, float yaw_i, float yaw_d, float pitch_p, float pitch_i, float pitch_d), void* data) {
	_on_pid_receive.callback = callback;
	_on_pid_receive.data = data;
}

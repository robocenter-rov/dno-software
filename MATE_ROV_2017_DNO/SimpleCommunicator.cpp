#include "SimpleCommunicator.h"
#include "DataReader.h"
#include "Hash.h"

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
};

SimpleCommunicator_t::SimpleCommunicator_t(ConnectionProvider_t* connection_provider) {
	_connection_provider = connection_provider;
}

void SimpleCommunicator_t::Begin() {
	
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
		DataReader_t dr(_connection_provider->Buffer(), readed_bytes);

		uint32_t msg_id;
		dr.Read(msg_id);

		if (msg_id > _last_received_msg_id) {
			_receive_packets_leak += msg_id - _last_received_msg_id - 1;
		}

		_last_received_msg_id = msg_id;

		while (dr.Available()) {
			uint8_t t;
			dr.Read(t);
			auto block_id = static_cast<RECEIVE_BLOCK_IDS>(t);

			switch (block_id) {
				case SBI_STATE:
					struct {
						State_t state;
						uint8_t last_i2c_scan;
					} state;
					dr.Read(state);
					_all_state = state.state;
					_last_received_i2c_scan_token = state.last_i2c_scan;
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
				case RBI_DEVICES_STATE:
					struct {
						float ArmPos;
						float HandPos;
						float M1;
						float M2;
						float Cam1Pos;
						float Cam2Pos;
					} devices_state;
					dr.Read(devices_state);
					_on_devices_state_receive.callback(_on_devices_state_receive.data, 
						devices_state.ArmPos,
						devices_state.HandPos,
						devices_state.M1,
						devices_state.M2,
						devices_state.Cam1Pos,
						devices_state.Cam2Pos
					);
				break;
				case RBI_MOTORS_STATE:
					struct {
						float M1;
						float M2;
						float M3;
						float M4;
						float M5;
						float M6;
					} motors_state;
					dr.Read(motors_state);
					_on_motors_state_receive.callback(_on_motors_state_receive.data,
						motors_state.M1,
						motors_state.M2,
						motors_state.M3,
						motors_state.M4,
						motors_state.M5,
						motors_state.M6
					);
				break;
				case RBI_MOVEMENT:
					struct {
						struct {
							int auto_depth : 1;
							int auto_yaw : 1;
							int auto_pitch : 1;
							int rest : 5;
						} control_type;
						float x;
						float y;
						float depth;
						float yaw;
						float pitch;
					} movement;
					dr.Read(movement);
					_on_movement_receive.callback(_on_movement_receive.data,
						movement.control_type.auto_depth,
						movement.control_type.auto_yaw,
						movement.control_type.auto_pitch,
						movement.x,
						movement.y,
						movement.depth,
						movement.yaw,
						movement.pitch
					);
				break;
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
					dr.Read(pids);

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
				default: ;
			}
		}
	}

	if (millis() - _last_msg_send_time < _send_frequency) {
		_connection_provider->BeginPacket();
		_connection_provider->Write(_last_sended_msg_id);

		_connection_provider->Write(SBI_STATE);

		bool flashlight_state;
		_on_state_need.callback(_on_state_need.data, flashlight_state);
		struct {
			int flashlight_state : 1;
		} state {flashlight_state};

		_connection_provider->Write(state);

		bool scanned; bool pca1; bool pca2; bool hmc58x3; bool itg3200; bool adxl345; bool bmp085; bool ms5803;
		_on_scanned_i2c_devices_need.callback(_on_scanned_i2c_devices_need.data,
			scanned, pca1, pca2, hmc58x3, itg3200, adxl345, bmp085, ms5803);

		struct {
			int pca1 : 1;
			int pca2 : 1;
			int hmc58x3 : 1;
			int itg3200 : 1;
			int adxl345 : 1;
			int bmp085 : 1;
			int ms5803 : 1;
		} scanned_i2c_devices{ pca1, pca2, hmc58x3, itg3200, adxl345, bmp085, ms5803 };

		if (scanned) {
			_last_i2c_scan_token = _last_received_i2c_scan_token;
		}

		_connection_provider->Write(_last_i2c_scan_token);
		_connection_provider->Write(scanned_i2c_devices);

		_connection_provider->Write(SBI_SENSOR_DATA);

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
			_connection_provider->Write(SBI_RAW_SENSOR_DATA);

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
			_connection_provider->Write(SBI_CALIBRATED_SENSOR_DATA);

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
			_connection_provider->Write(SBI_PID_STATE_RECEIVE);

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
			_connection_provider->Write(SBI_MOTORS_STATE_RECEIVE);

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
			_connection_provider->Write(SBI_BLUETOOTH_MSG_RECEIVE);

			char* bluetooth_message;

			_on_bluetooth_msg_need.callback(_on_bluetooth_msg_need.data, bluetooth_message);

			_connection_provider->Write(bluetooth_message, 7);
		}
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

void SimpleCommunicator_t::OnBluetoothMessageNeed(void(* callback)(void* data, char*& bluetooth_message), void* data) {
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

void SimpleCommunicator_t::OnMovementReceive(void(* callback)(void* data, bool auto_depth, bool auto_yaw, bool auto_pitch, float x, float y, float depth, float yaw, float pitch), void* data) {
	_on_movement_receive.callback = callback;
	_on_movement_receive.data = data;
}

void SimpleCommunicator_t::OnPidReceive(void(* callback)(void* data, float depth_p, float depth_i, float depth_d, float yaw_p, float yaw_i, float yaw_d, float pitch_p, float pitch_i, float pitch_d), void* data) {
	_on_pid_receive.callback = callback;
	_on_pid_receive.data = data;
}

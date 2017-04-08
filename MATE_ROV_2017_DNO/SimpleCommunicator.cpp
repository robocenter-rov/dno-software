#include "SimpleCommunicator.h"
#include "DataReader.h"

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
						struct {
							int flashlight_state : 1;
							int read_bluetooth : 1;
							int send_raw_sensor_data : 1;
							int send_calibrated_sensor_data : 1;
							int send_pid_state : 1;
							int send_motors_state : 1;
							int rest : 2;
						} state;
						uint8_t last_i2c_scan;
					} state;
					dr.Read(state);
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
					Pids_t pids;
					dr.Read(pids);
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
				break;
				default: ;
			}
		}
	}

	if (millis() - _last_msg_send_time < _send_frequency) {
		_connection_provider->BeginPacket();
		_connection_provider->Write(_last_sended_msg_id);

		_connection_provider->Write(SBI_STATE);
		_connection_provider->Write(_all_state);
		_connection_provider->Write(_scanned_i2c_devices);
		_connection_provider->Write(_last_i2c_scan_token);

		_connection_provider->Write(SBI_SENSOR_DATA);
		_connection_provider->Write(_sensor_data);

		if (_all_state.send_raw_sensor_data) {
			_connection_provider->Write(SBI_RAW_SENSOR_DATA);
			_connection_provider->Write(_raw_sensor_data);
		}

		if (_all_state.send_calibrated_sensor_data) {
			_connection_provider->Write(SBI_CALIBRATED_SENSOR_DATA);
			_connection_provider->Write(_calibrated_sensor_data);
		}

		if (_all_state.send_pid_state) {
			_connection_provider->Write(SBI_PID_STATE_RECEIVE);
			_connection_provider->Write(_pids_state);
		}

		if (_all_state.send_motors_state) {
			_connection_provider->Write(SBI_MOTORS_STATE_RECEIVE);
			_connection_provider->Write(_motors_state);
		}

		if (_all_state.read_bluetooth) {
			_connection_provider->Write(SBI_BLUETOOTH_MSG_RECEIVE);
			_connection_provider->Write(_bluetooth_message, 7);
		}
	}

	return 0;
}

void SimpleCommunicator_t::SetState(bool flashlight_state) {
	_all_state.flashlight_state = flashlight_state;
}

void SimpleCommunicator_t::SetSensorData(float q1, float q2, float q3, float q4, float depth) {
	_sensor_data.q1 = q1;
	_sensor_data.q2 = q2;
	_sensor_data.q3 = q3;
	_sensor_data.q4 = q4;
	_sensor_data.depth = depth;
}

void SimpleCommunicator_t::SetRawSensorData(int ax, int ay, int az, int gx, int gy, int gz, int mx, int my, int mz, float depth) {
	_raw_sensor_data.ax = ax;
	_raw_sensor_data.ay = ay;
	_raw_sensor_data.az = az;

	_raw_sensor_data.gx = gx;
	_raw_sensor_data.gy = gy;
	_raw_sensor_data.gz = gz;

	_raw_sensor_data.mx = mx;
	_raw_sensor_data.my = my;
	_raw_sensor_data.mz = mz;

	_raw_sensor_data.depth = depth;
}

void SimpleCommunicator_t::SetCalibratedSensorData(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz) {
	_calibrated_sensor_data.ax = ax;
	_calibrated_sensor_data.ay = ay;
	_calibrated_sensor_data.az = az;

	_calibrated_sensor_data.gx = gx;
	_calibrated_sensor_data.gy = gy;
	_calibrated_sensor_data.gz = gz;
	
	_calibrated_sensor_data.mx = mx;
	_calibrated_sensor_data.my = my;
	_calibrated_sensor_data.mz = mz;
}

void SimpleCommunicator_t::SetBluetoothMessage(char* bluetooth_message) {
	memcpy(_bluetooth_message, bluetooth_message, 7);
}

void SimpleCommunicator_t::SetPids(float depth_p, float depth_i, float depth_d, float yaw_p, float yaw_i, float yaw_d, float pitch_p, float pitch_i, float pitch_d) {
	_pids.depth_p = depth_p;
	_pids.depth_i = depth_i;
	_pids.depth_d = depth_d;

	_pids.yaw_p = yaw_p;
	_pids.yaw_i = yaw_i;
	_pids.yaw_d = yaw_d;

	_pids.pitch_p = pitch_p;
	_pids.pitch_i = pitch_i;
	_pids.pitch_d = pitch_d;
}

void SimpleCommunicator_t::SetMotorsState(float m1, float m2, float m3, float m4, float m5, float m6) {
	_motors_state.m1 = m1;
	_motors_state.m2 = m2;
	_motors_state.m3 = m3;
	_motors_state.m4 = m4;
	_motors_state.m5 = m5;
	_motors_state.m6 = m6;
}

void SimpleCommunicator_t::SetScannedI2CDevices(bool pca1, bool pca2, bool hmc58x3, bool itg3200, bool adxl345, bool bmp085, bool ms5803) {
	_scanned_i2c_devices.pca1 = pca1;
	_scanned_i2c_devices.pca2 = pca2;
	_scanned_i2c_devices.hmc58x3 = hmc58x3;
	_scanned_i2c_devices.itg3200 = itg3200;
	_scanned_i2c_devices.adxl345 = adxl345;
	_scanned_i2c_devices.bmp085 = bmp085;
	_scanned_i2c_devices.ms5803 = ms5803;

	_last_i2c_scan_token = _last_received_i2c_scan_token;
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

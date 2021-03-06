#pragma once

#include "Arduino.h"
#include "ConnectionProvider.h"

class SimpleCommunicator_t {
private:
	struct BaseCallback_t {
		void* data;
	};

	struct :BaseCallback_t {
		void(*callback)(void* data);
	} _on_connect;

	struct :BaseCallback_t {
		void(*callback)(void* data);
	} _on_disconnect;

	struct :BaseCallback_t {
		void(*callback)(void* data, bool flashlight_state, bool read_bluetooth, bool send_raw_sensor_data, bool send_calibrated_sensor_data, bool send_pid_state, bool send_motors_state, uint8_t i2c_scan_token);
	} _on_state_receive;

	struct :BaseCallback_t {
		void(*callback)(void* data, float arm_pos, float hand_pos, float m1_pos, float m2_pos, float cam1_pos, float cam2_pos, bool cam1local, bool cam2local);
	} _on_devices_state_receive;

	struct :BaseCallback_t {
		void(*callback)(void* data, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8);
	} _on_motors_state_receive;

	struct :BaseCallback_t {
		void(*callback)(void* data, bool auto_yaw, bool auto_pitch, bool auto_roll, bool auto_depth, float local_x, float local_y, float local_z, float global_x, float global_y, float global_z, float yaw, float pitch, float roll);
	} _on_movement_receive;

	struct :BaseCallback_t {
		void(*callback)(void* data, int m1pos, int m2pos, int m3pos, int m4pos, int m5pos, int m6pos, int m7pos, int m8pos, float m1mul, float m2mul, float m3mul, float m4mul, float m5mul, float m6mul, float m7mul, float m8mul);
	} _on_motors_config_receive;

	struct :BaseCallback_t {
		void(*callback)(void* data, float depth_p, float depth_i, float depth_d, float yaw_p, float yaw_i, float yaw_d, float pitch_p, float pitch_i, float pitch_d, float roll_p, float roll_i, float roll_d);
	} _on_pid_receive;

	struct :BaseCallback_t {
		void(*callback)(void* data, bool& flashlight_state);
	} _on_state_need;

	struct :BaseCallback_t {
		void(*callback)(void* data, float& q1, float& q2, float& q3, float& q4, float& depth);
	} _on_sensor_data_need;

	struct :BaseCallback_t {
		void(*callback)(void* data, int& ax, int& ay, int& az, int& gx, int& gy, int& gz, int& mx, int& my, int& mz, float& depth);
	} _on_raw_sensor_data_need;

	struct :BaseCallback_t {
		void(*callback)(void* data, float& ax, float& ay, float& az, float& gx, float& gy, float& gz, float& mx, float& my, float& mz, float& depth);
	} _on_calibrated_sensor_data_need;

	struct :BaseCallback_t {
		void(*callback)(void* data, char* bluetooth_message, bool& readed);
	} _on_bluetooth_msg_need;

	struct :BaseCallback_t {
		void(*callback)(void* data, float& depth_in, float& depth_target, float& depth_out, float& yaw_in, float& yaw_target, float& yaw_out, float& pitch_in, float& pitch_out, float& pitch_target, float& roll_in, float& roll_out, float& roll_target);
	} _on_pids_state_need;

	struct :BaseCallback_t {
		void(*callback)(void* data, float& m1, float& m2, float& m3, float& m4, float& m5, float& m6, float& m7, float& m8);
	} _on_motors_state_need;

	struct :BaseCallback_t {
		void(*callback)(void* data, bool& scanned, bool& pca1, bool& pca2, bool& pca3, bool& hmc58x3, bool& itg3200, bool& adxl345, bool& bmp085, bool& ms5803);
	} _on_scanned_i2c_devices_need;

	struct :BaseCallback_t {
		void(*callback)(void* data, float camera1_maxVal, float camera1_minVal, float camera2_maxVal, float camera2_minVal);
	} _on_cameras_config_receive;

	struct :BaseCallback_t {
		void(*callback)(void* data, float acc_x_bias, float acc_y_bias, float acc_z_bias, float acc_x_scale, float acc_y_scale, float acc_z_scale, float gyro_x_bias, float gyro_y_bias, float gyro_z_bias, float gyro_scale);
	} _on_imu_config_receive;

	struct :BaseCallback_t {
		void(*callback)(void* data, int stabilization_update_frequency);
	} _on_stabilization_update_frequency_receive;

	struct State_t {
		bool flashlight_state : 1;
		bool read_bluetooth : 1;
		bool send_raw_sensor_data : 1;
		bool send_calibrated_sensor_data : 1;
		bool send_pid_state : 1;
		bool send_motors_state : 1;
		bool rest : 2;
	} _all_state;

	struct ScannedI2CDevices_t {
		bool pca1 : 1;
		bool pca2 : 1;
		bool pca3 : 1;
		bool hmc58x3 : 1;
		bool itg3200 : 1;
		bool adxl345 : 1;
		bool bmp085 : 1;
		bool ms5803 : 1;
	} _last_scanned_i2c_devices;

	uint8_t _last_i2c_scan_token;
	uint8_t _last_received_i2c_scan_token;

	uint32_t _config_hash;

	unsigned long _last_msg_receive_time;
	unsigned long _last_msg_send_time;

	unsigned int _send_frequency;
	unsigned int _receive_time_out;

	unsigned long _last_update_time;

	uint32_t _last_received_msg_id;
	uint32_t _last_sended_msg_id;
	uint16_t _receive_packets_leak;

	bool _connected;

	ConnectionProvider_t* _connection_provider;
public:
	SimpleCommunicator_t(ConnectionProvider_t* connection_provider);
	int Begin();
	void Stop();
	bool Connected() const;

	int Update();

	void OnStateNeed(void(*callback)(void* data, bool& flashlight_state), void* data);
	void OnSensorDataNeed(void(*callback)(void* data, float& q1, float& q2, float& q3, float& q4, float& depth), void* data);
	void OnRawSensorDataNeed(void(*callback)(void* data, int& ax, int& ay, int& az, int& gx, int& gy, int& gz, int& mx, int& my, int& mz, float& depth), void* data);
	void OnCalibratedSensorDataNeed(void(*callback)(void* data, float& ax, float& ay, float& az, float& gx, float& gy, float& gz, float& mx, float& my, float& mz, float& depth), void* data);
	void OnBluetoothMessageNeed(void(*callback)(void* data, char* bluetooth_message, bool& readed), void* data);
	void OnPidsStateNeed(void(*callback)(void* data, float& depth_in, float& depth_target, float& depth_out, float& yaw_in, float& yaw_target, float& yaw_out, float& pitch_in, float& pitch_out, float& pitch_target, float& roll_in, float& rol_out, float& roll_target), void* data);
	void OnMotorsStateNeed(void(*callback)(void* data, float& m1, float& m2, float& m3, float& m4, float& m5, float& m6, float& m7, float& m8), void* data);
	void OnScannedI2CDevicesNeed(void(*callback)(void* data, bool& scanned, bool& pca1, bool& pca2, bool& pca3, bool& hmc58x3, bool& itg3200, bool& adxl345, bool& bmp085, bool& ms5803), void* data);

	void OnConnect(void(*callback)(void* data), void* data);
	void OnDisconnect(void(*callback)(void* data), void* data);
	void OnStateReceive(void(*callback)(void* data, bool flashlight_state, bool read_bluetooth, bool send_raw_sensor_data, bool send_calibrated_sensor_data, bool send_pid_state, bool send_motors_state, uint8_t i2c_scan_token), void* data);
	void OnDevicesStateReceive(void(*callback)(void* data, float arm_pos, float hand_pos, float m1_pos, float m2_pos, float cam1_pos, float cam2_pos, bool cam1local, bool cam2local), void* data);
	void OnMotorsStateReceive(void(*callback)(void* data, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8), void* data);
	void OnMotorsConfigReceive(void(*callback)(void* data, int m1pos, int m2pos, int m3pos, int m4pos, int m5pos, int m6pos, int m7pos, int m8pos, float m1mul, float m2mul, float m3mul, float m4mul, float m5mul, float m6mul, float m7mul, float m8mul), void* data);
	void OnMovementReceive(void(*callback)(void* data, bool auto_yaw, bool auto_pitch, bool auto_roll, bool auto_depth, float local_x, float local_y, float local_z, float global_x, float global_y, float global_z, float yaw, float pitch, float roll), void* data);
	void OnPidReceive(void(*callback)(void* data, float depth_p, float depth_i, float depth_d, float yaw_p, float yaw_i, float yaw_d, float pitch_p, float pitch_i, float pitch_d, float roll_p, float roll_i, float roll_d), void* data);
	void OnCamerasConfigReceive(void(*callback)(void* data, float camera1_maxVal, float camera1_minVal, float camera2_maxVal, float camera2_minVal), void* data);
	void OnIMUConfigReceive(void(*callback)(void* data, float acc_x_bias, float acc_y_bias, float acc_z_bias, float acc_x_scale, float acc_y_scale, float acc_z_scale, float gyro_x_bias, float gyro_y_bias, float gyro_z_bias, float gyro_scale), void* data);
	void OnStabilizationUpdateFrequency(void(*callback)(void *data, int stabilization_update_frequency), void* data);
};
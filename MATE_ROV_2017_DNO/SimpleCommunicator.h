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
		void(*callback)(void* data, float arm_pos, float hand_pos, float m1_pos, float m2_pos, float cam1_pos, float cam2_pos);
	} _on_devices_state_receive;

	struct :BaseCallback_t {
		void(*callback)(void* data, float m1, float m2, float m3, float m4, float m5, float m6);
	} _on_motors_state_receive;

	struct :BaseCallback_t {
		void(*callback)(void* data, bool auto_depth, bool auto_yaw, bool auto_pitch, float x, float y, float depth, float yaw, float pitch);
	} _on_movement_receive;

	struct :BaseCallback_t {
		void(*callback)(void* data, float depth_p, float depth_i, float depth_d, float yaw_p, float yaw_i, float yaw_d, float pitch_p, float pitch_i, float pitch_d);
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
		void(*callback)(void* data, float& depth_in, float& depth_target, float& depth_out, float& yaw_in, float& yaw_target, float& yaw_out, float& pitch_in, float& pitch_out, float& pitch_target);
	} _on_pids_state_need;

	struct :BaseCallback_t {
		void(*callback)(void* data, float& m1, float& m2, float& m3, float& m4, float& m5, float& m6);
	} _on_motors_state_need;

	struct :BaseCallback_t {
		void(*callback)(void* data, bool& scanned, bool& pca1, bool& pca2, bool& hmc58x3, bool& itg3200, bool& adxl345, bool& bmp085, bool& ms5803);
	} _on_scanned_i2c_devices_need;

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
		bool hmc58x3 : 1;
		bool itg3200 : 1;
		bool adxl345 : 1;
		bool bmp085 : 1;
		bool ms5803 : 1;
	} _last_scanned_i2c_devices;

	uint8_t _last_i2c_scan_token;
	uint8_t _last_received_i2c_scan_token;

	uint32_t _pids_hash;

	unsigned long _last_msg_receive_time;
	unsigned long _last_msg_send_time;

	unsigned int _send_frequency;
	unsigned int _receive_time_out;

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
	void OnPidsStateNeed(void(*callback)(void* data, float& depth_in, float& depth_target, float& depth_out, float& yaw_in, float& yaw_target, float& yaw_out, float& pitch_in, float& pitch_out, float& pitch_target), void* data);
	void OnMotorsStateNeed(void(*callback)(void* data, float& m1, float& m2, float& m3, float& m4, float& m5, float& m6), void* data);
	void OnScannedI2CDevicesNeed(void(*callback)(void* data, bool& scanned, bool& pca1, bool& pca2, bool& hmc58x3, bool& itg3200, bool& adxl345, bool& bmp085, bool& ms5803), void* data);

	void OnConnect(void(*callback)(void* data), void* data);
	void OnDisconnect(void(*callback)(void* data), void* data);
	void OnStateReceive(void(*callback)(void* data, bool flashlight_state, bool read_bluetooth, bool send_raw_sensor_data, bool send_calibrated_sensor_data, bool send_pid_state, bool send_motors_state, uint8_t i2c_scan_token), void* data);
	void OnDevicesStateReceive(void(*callback)(void* data, float arm_pos, float hand_pos, float m1_pos, float m2_pos, float cam1_pos, float cam2_pos), void* data);
	void OnMotorsStateReceive(void(*callback)(void* data, float m1, float m2, float m3, float m4, float m5, float m6), void* data);
	void OnMovementReceive(void(*callback)(void* data, bool auto_depth, bool auto_yaw, bool auto_pitch, float x, float y, float depth, float yaw, float pitch), void* data);
	void OnPidReceive(void(*callback)(void* data, float depth_p, float depth_i, float depth_d, float yaw_p, float yaw_i, float yaw_d, float pitch_p, float pitch_i, float pitch_d), void* data);
};
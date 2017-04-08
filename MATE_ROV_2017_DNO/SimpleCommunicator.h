#pragma once

class SimpleCommunicator_t {
private:
	struct BaseCallback_t {
		void* data;
	};
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
		uint8_t last_i2c_scan_token;
		struct {
			int pca1 : 1;
			int pca2 : 1;
			int hmc58x3 : 1;
			int itg3200 : 1;
			int adxl345 : 1;
			int bmp085 : 1;
			int ms5803 : 1;
		} scanned_devices;
		uint32_t pid_hash;
	} _all_state;

	struct {
		float q1;
		float q2;
		float q3;
		float q4;
		float depth;
	} _sensor_data;

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
	} _raw_sensor_data;

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
	} _calibrated_sensor_data;

	char _bluetooth_message[7];

	struct Pids_t {
		float depth_p;
		float depth_i;
		float depth_d;

		float yaw_p;
		float yaw_i;
		float yaw_d;

		float pitch_p;
		float pitch_i;
		float pitch_d;
	} _pids;

	struct {
		float m1;
		float m2;
		float m3;
		float m4;
		float m5;
		float m6;
	} _motors_state;

	uint32_t _pids_hash;

	unsigned long _last_msg_receive_time;

	uint32_t _last_received_msg_id;
	uint16_t _receive_packets_leak;

	ConnectionProvider_t* _connection_provider;
public:
	SimpleCommunicator_t(ConnectionProvider_t* connection_provider);
	void Begin();
	void Stop();
	bool Connected();

	int Update();

	void SetState(bool flashlight_state, bool read_bluetooth, 
		uint8_t last_i2c_scan_token, 
		bool pca1, bool pca2, bool hmc58x3, bool itg3200, bool adxl345, bool bmp085, bool ms5803
	);
	void SetSensorData(float q1, float q2, float q3, float q4, float depth);
	void SetRawSensorData(int ax, int ay, int az, int gx, int gy, int gz, int mx, int my, int mz);
	void SetCalibratedSensorData(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);
	void SetBluetoothMessage(char* bluetooth_message);
	void SetPids(float depth_p, float depth_i, float depth_d, float yaw_p, float yaw_i, float yaw_d, float pitch_p, float pitch_i, float pitch_d);
	void SetMotorsState(float m1, float m2, float m3, float m4, float m5, float m6);

	void OnStateReceive(void(*callback)(void* data, bool flashlight_state, bool read_bluetooth, bool send_raw_sensor_data, bool send_calibrated_sensor_data, bool send_pid_state, bool send_motors_state, uint8_t i2c_scan_token), void* data);
	void OnDevicesStateReceive(void(*callback)(void* data, float arm_pos, float hand_pos, float m1_pos, float m2_pos, float cam1_pos, float cam2_pos), void* data);
	void OnMotorsStateReceive(void(*callback)(void* data, float m1, float m2, float m3, float m4, float m5, float m6), void* data);
	void OnMovementReceive(void(*callback)(void* data, bool auto_depth, bool auto_yaw, bool auto_pitch, float x, float y, float depth, float yaw, float pitch), void* data);
	void OnPidReceive(void(*callback)(void* data, float depth_p, float depth_i, float depth_d, float yaw_p, float yaw_i, float yaw_d, float pitch_p, float pitch_i, float pitch_d), void* data);
};
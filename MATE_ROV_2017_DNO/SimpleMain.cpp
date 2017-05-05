#include "SimpleMain.h"

SimpleMain_t::SimpleMain_t(SimpleCommunicator_t* communicator, Movement_t* movement,
	SensorManager_t* sensor_manager, PeripheryManager_t* periphery_manager)
{
	_communicator = communicator;
	_movement = movement;
	_sensor_manager = sensor_manager;
	_periphery_manager = periphery_manager;

	_communicator->OnStateNeed([](void* data, bool& flashlight_state)
	{
		auto main = static_cast<SimpleMain_t*>(data);

		flashlight_state = main->_periphery_manager->GetFlashlightState();
	}, this);

	_communicator->OnBluetoothMessageNeed([](void* data, char* msg, bool& readed)
	{
		auto main = static_cast<SimpleMain_t*>(data);
		readed = main->_periphery_manager->ReceiveBluetoothMessage(msg);
	}, this);

	_communicator->OnSensorDataNeed([](void* data, float& q1, float& q2, float& q3, float& q4, float& depth)
	{
		auto main = static_cast<SimpleMain_t*>(data);

		main->_sensor_manager->GetRotation(q1, q2, q3, q4);

		depth = main->_sensor_manager->GetDepth();
	}, this);

	_communicator->OnRawSensorDataNeed([](void* data, int& ax, int& ay, int& az, int& gx, int& gy, int& gz, int& mx, int& my, int& mz, float& depth)
	{
		auto main = static_cast<SimpleMain_t*>(data);

		int raw_sensor_data[10];
		main->_sensor_manager->GetRawRotation(raw_sensor_data);

		ax = raw_sensor_data[0];
		ay = raw_sensor_data[1];
		az = raw_sensor_data[2];

		gx = raw_sensor_data[3];
		gy = raw_sensor_data[4];
		gz = raw_sensor_data[5];

		mx = raw_sensor_data[6];
		my = raw_sensor_data[7];
		mz = raw_sensor_data[8];

		depth = raw_sensor_data[9];
	}, this);

	_communicator->OnCalibratedSensorDataNeed([](void* data, float& ax, float& ay, float& az, float& gx, float& gy, float& gz, float& mx, float& my, float& mz, float& depth)
	{
		auto main = static_cast<SimpleMain_t*>(data);

		float sensor_data[10];
		main->_sensor_manager->GetCalibratedRotation(sensor_data);

		ax = sensor_data[0];
		ay = sensor_data[1];
		az = sensor_data[2];

		gx = sensor_data[3];
		gy = sensor_data[4];
		gz = sensor_data[5];

		mx = sensor_data[6];
		my = sensor_data[7];
		mz = sensor_data[8];

		depth = main->_sensor_manager->GetDepth();
	}, this);

	_communicator->OnMotorsStateNeed([](void* data, float& m1, float& m2, float& m3, float& m4, float& m5, float& m6)
	{
		auto main = static_cast<SimpleMain_t*>(data);

		m1 = main->_movement->GetMotorThrust(0);
		m2 = main->_movement->GetMotorThrust(1);
		m3 = main->_movement->GetMotorThrust(2);
		m4 = main->_movement->GetMotorThrust(3);
		m5 = main->_movement->GetMotorThrust(4);
		m6 = main->_movement->GetMotorThrust(5);
	}, this);

	_communicator->OnScannedI2CDevicesNeed([](void* data, bool& scanned, bool& pca1, bool& pca2, bool& hmc58x3, bool& itg3200, bool& adxl345, bool& bmp085, bool& ms5803)
	{
		uint8_t adresses[] = {
			0x40, // PCA1
			0x41, // PCA2
			0x1E, // HMC58X3
			0x53, // ADXL345
			0x68, // ITG3200
			0x77, // BMP085
			0x76, // MS5803
		};

		bool s[sizeof(adresses)];

		for (int i = 0; i < sizeof(adresses); i++) {
			Wire.beginTransmission(adresses[i]);
			s[i] = Wire.endTransmission() == 0;
		}

		pca1 = s[0];
		pca2 = s[1];
		hmc58x3 = s[2];
		itg3200 = s[3];
		adxl345 = s[4];
		bmp085 = s[5];
		ms5803 = s[6];
	}, this);

	_communicator->OnDevicesStateReceive([](void* data, float arm_pos, float hand_pos, float m1_pos, 
											float m2_pos, float cam1_pos, float cam2_pos) -> void {
		
		auto main = static_cast<SimpleMain_t*>(data);

		main->_periphery_manager->GetManipulator()->SetArmThrust(arm_pos);
		main->_periphery_manager->GetManipulator()->SetHandThrust(hand_pos);

		main->_periphery_manager->SetAngleM1(m1_pos);
		main->_periphery_manager->SetAngleM2(m2_pos);

		main->_periphery_manager->SetAngleCamera1(cam1_pos);
		main->_periphery_manager->SetAngleCamera2(cam2_pos);

	}, this);

	_communicator->OnMotorsStateReceive([](void* data, float m1, float m2, float m3, float m4, float m5, float m6)->void {
		auto main = static_cast<SimpleMain_t*>(data);

		LOGLN("OnMotorsStateReceive");
		LOG("motor1: ");
		LOGLN(m1);
		LOG("motor2: ");
		LOGLN(m2);
		LOG("motor3: ");
		LOGLN(m3);
		LOG("motor4: ");
		LOGLN(m4);
		LOG("motor5: ");
		LOGLN(m5);
		LOG("motor6: ");
		LOGLN(m6);

		main->_movement->SetMotorThrust(0, m1);
		main->_movement->SetMotorThrust(1, m2);
		main->_movement->SetMotorThrust(2, m3);
		main->_movement->SetMotorThrust(3, m4);
		main->_movement->SetMotorThrust(4, m5);
		main->_movement->SetMotorThrust(5, m6);

	}, this);

	_communicator->OnMovementReceive([](void* data, bool auto_depth, bool auto_yaw, bool auto_pitch, float x, float y, float depth, float yaw, float pitch)->void {
		auto main = static_cast<SimpleMain_t*>(data);

		if (auto_depth)
		{
			auto_pitch = true;
			main->_movement->SetDepth(depth);
		}
		if (auto_yaw)
		{
			
		}
		if (auto_pitch)
		{
			
		}

		main->_movement->

	}, this);

	_communicator->OnPidReceive([](void* data, float depth_p, float depth_i, float depth_d, float yaw_p, float yaw_i, float yaw_d, float pitch_p, float pitch_i, float pitch_d)-> void {

	}, this);

	_communicator->OnStateReceive([](void* data, bool flashlight_state, bool read_bluetooth, bool send_raw_sensor_data, 
		bool send_calibrated_sensor_data, bool send_pid_state, bool send_motors_state, uint8_t i2c_scan_token)->void{
		auto main = static_cast<SimpleMain_t*>(data);

		LOG("Flashlight state: ");
		LOGLN(flashlight_state);

		LOG("Read bluetooth: ");
		LOGLN(read_bluetooth);

		LOG("Send raw sensor data: ");
		LOGLN(send_raw_sensor_data);

		LOG("Send calibrated sensor data: ");
		LOGLN(send_calibrated_sensor_data);

		LOG("Send pid state: ");
		LOGLN(send_pid_state);

		LOG("Send motors state: ");
		LOGLN(send_motors_state);

		LOG("I2C scan token: ");
		LOGLN(i2c_scan_token);

		main->_periphery_manager->SetFlashlightState(flashlight_state);
		if (send_raw_sensor_data)
		{
			
		}
		if (send_calibrated_sensor_data)
		{
			
		}
		if (send_pid_state)
		{
			
		}
		if (send_motors_state)
		{
			//main->_communicator->SetMotorsState()
		}
	}, this);
}

SimpleMain_t::~SimpleMain_t()
{
}

int SimpleMain_t::Begin()
{
	return _communicator->Begin();
}

void SimpleMain_t::Loop()
{
	_sensor_manager->Update();
	_communicator->Update();
}

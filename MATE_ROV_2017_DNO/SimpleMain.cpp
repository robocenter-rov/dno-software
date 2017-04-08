#include "SimpleMain.h"

SimpleMain_t::SimpleMain_t(SimpleCommunicator_t* communicator, Movement_t* movement,
	SensorManager_t* sensor_manager, PeripheryManager_t* periphery_manager)
{
	_communicator = communicator;
	_movement = movement;
	_sensor_manager = sensor_manager;
	_periphery_manager = periphery_manager;


	_communicator->OnDevicesStateReceive([](void* data, float arm_pos, float hand_pos, float m1_pos, float m2_pos, float cam1_pos, float cam2_pos) -> void {
		auto main = static_cast<SimpleMain_t*>(data);

		//main->_movement->
	}, this);

	_communicator->OnMotorsStateReceive([](void* data, float m1, float m2, float m3, float m4, float m5, float m6)->void {
		auto main = static_cast<SimpleMain_t*>(data);

		Serial.println("OnMotorsStateReceive");
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

	}, this);

	_communicator->OnPidReceive([](void* data, float depth_p, float depth_i, float depth_d, float yaw_p, float yaw_i, float yaw_d, float pitch_p, float pitch_i, float pitch_d)-> void {

	}, this);

	_communicator->OnStateReceive([](void* data, bool flashlight_state, bool read_bluetooth, bool send_raw_sensor_data, 
		bool send_calibrated_sensor_data, bool send_pid_state, bool send_motors_state, uint8_t i2c_scan_token)->void{
		auto main = static_cast<SimpleMain_t*>(data);

		main->_periphery_manager->SetFlashlightState(flashlight_state);
		main->_periphery_manager->ReceiveBluetoothMessage(read_bluetooth);
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
	_communicator->Update();
}

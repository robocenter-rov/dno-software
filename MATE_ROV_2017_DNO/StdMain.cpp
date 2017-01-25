#include "StdMain.h"

StdMain_t::StdMain_t(Communicator_t* communicator, Movement_t* movement,
	SensorManager_t* sensor_manager, PeripheryManager_t* periphery_manager)
	: _task_pool(3) {
	_communicator = communicator;
	_movement = movement;
	_sensor_manager = sensor_manager;
	_periphery_manager = periphery_manager;

	_communicator->SetOnSetFlashlightStateReceive([](void* data, unsigned int tag, byte state) -> int {
		auto main = static_cast<StdMain_t*>(data);

#ifdef _DEBUG
		Serial.println("SetOnSetFlashlightStateReceive");
		Serial.print("tag: ");
		Serial.println(tag);
		Serial.print("state: ");
		Serial.println(state);
#endif

		return main->AddTask(new SetFlashlightStateTask_t(tag, state, main->_periphery_manager));
	}, this);

	_communicator->SetOnBlinkFlashlightReceive([](void* data, unsigned int tag, unsigned int count) -> int {
		auto main = static_cast<StdMain_t*>(data);

#ifdef _DEBUG
		Serial.println("SetOnBlinkFlashlightReceive");
		Serial.print("tag: ");
		Serial.println(tag);
		Serial.print("count: ");
		Serial.println(count);
#endif

		return main->AddTask(new BlinkFlashlightTask_t(tag, count, main->_periphery_manager));
	}, this);

	_communicator->SetOnCancelTaskReceive([](void* data, unsigned int worker_id) -> int {
		auto main = static_cast<StdMain_t*>(data);

#ifdef _DEBUG
		Serial.println("SetOnCancelTaskReceive");
		Serial.print("worker id: ");
		Serial.println(worker_id);
#endif

		return main->_task_pool.FreeWorker(worker_id);
	}, this);

	_communicator->SetOnSetMotorsThrustReceive([](void* data, unsigned int motor1Thrust, unsigned int motor2Thrust, unsigned int motor3Thrust, 
		unsigned int motor4Thrust, unsigned int motor5Thrust, unsigned int motor6Thrust) -> int {
		auto main = static_cast<StdMain_t*>(data);

#ifdef _DEBUG
		Serial.println("SetOnBlinkFlashlightReceive");
		Serial.print("tag: ");
		Serial.println(tag);
		Serial.print("count: ");
		Serial.println(count);
#endif

		main->_movement->SetMotorThrust(0, motor1Thrust / 4096.f);
		main->_movement->SetMotorThrust(1, motor2Thrust / 4096.f);
		main->_movement->SetMotorThrust(2, motor3Thrust / 4096.f);
		main->_movement->SetMotorThrust(3, motor4Thrust / 4096.f);
		main->_movement->SetMotorThrust(4, motor5Thrust / 4096.f);
		main->_movement->SetMotorThrust(5, motor6Thrust / 4096.f);

		return 0;
	}, this);
}

int StdMain_t::Begin() {
	return _communicator->Begin();
}

void StdMain_t::SendTaskState(unsigned int worker_id) const {

#ifdef _DEBUG
	Serial.print("Sending state of worker, id: ");
	Serial.println(worker_id);
#endif

	_communicator->SendTaskState(_task_pool.GetTaskStatePtr(worker_id));
}

int StdMain_t::AddTask(Task_t* task) {
	if (_task_pool.AddTask(task)) {
		if (_communicator->SendException(Exceptions::GetLastException())) {
			return 1;
		}
		Exceptions::Release();
	} else {
		if (_communicator->SendTaskState(_task_pool.GetLastAddedTaskStatePtr())) {
			return 1;
		}
	}
	return 0;
}

void StdMain_t::Loop() {
	if (_communicator->Update()) {
		_communicator->SendException(Exceptions::GetLastException());
		Exceptions::Release();
	}
	_task_pool.Update();
}

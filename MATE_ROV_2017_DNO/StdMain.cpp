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

#include "StdMain.h"
#include "MathConstants.h"
#include "BlinkFlashLightTask.h"
#include "SetFlashlightStateTask.h"
#include "SendSensorDataTask.h"
#include "ReceiveBluetoothMessageTask.h"

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

	_communicator->SetOnCancelTaskReceive([](void* data, int worker_id) -> int {
		auto main = static_cast<StdMain_t*>(data);

#ifdef _DEBUG
		Serial.println("SetOnCancelTaskReceive");
		Serial.print("worker id: ");
		Serial.println(worker_id);
#endif

		return main->_task_pool.FreeWorker(worker_id);
	}, this);

	_communicator->SetOnSetMotorsThrustReceive([](void* data, int motor1Thrust, int motor2Thrust, int motor3Thrust, 
		int motor4Thrust, int motor5Thrust, int motor6Thrust) -> int {
		auto main = static_cast<StdMain_t*>(data);

#ifdef _DEBUG
		Serial.println("SetMotorsThrustReceive");
		Serial.print("motor1: ");
		Serial.println(motor1Thrust);
		Serial.print("motor2: ");
		Serial.println(motor2Thrust);
		Serial.print("motor3: ");
		Serial.println(motor3Thrust);
		Serial.print("motor4: ");
		Serial.println(motor4Thrust);
		Serial.print("motor5: ");
		Serial.println(motor5Thrust);
		Serial.print("motor6: ");
		Serial.println(motor6Thrust);
#endif

		main->_movement->SetMotorThrust(0, motor1Thrust / 4096.f);
		main->_movement->SetMotorThrust(1, motor2Thrust / 4096.f);
		main->_movement->SetMotorThrust(2, motor3Thrust / 4096.f);
		main->_movement->SetMotorThrust(3, motor4Thrust / 4096.f);
		main->_movement->SetMotorThrust(4, motor5Thrust / 4096.f);
		main->_movement->SetMotorThrust(5, motor6Thrust / 4096.f);

		return 0;
	}, this);

	_communicator->SetOnSetManipulatorPositionReceive([](void* data, unsigned int handPosition, unsigned int armPosition) -> int {
		auto main = static_cast<StdMain_t*>(data);

#ifdef _DEBUG
		Serial.println("SetManipulatorPositionReceive");
		Serial.print("handPosition: ");
		Serial.println(handPosition);
		Serial.print("armPosition: ");
		Serial.println(armPosition);
#endif

		main->_periphery_manager->GetManipulator()->SetHandAngle(constrain(handPosition, 0, 4095) / 4095.f * PI2);
		main->_periphery_manager->GetManipulator()->SetArmAngle(constrain(armPosition, 0, 4095) / 4095.f * PI2);

		return 0;
	}, this);

	_communicator->SetOnStartSendingSensorDataReceive([](void* data, unsigned int tag, unsigned int interval) -> int {
		auto main = static_cast<StdMain_t*>(data);

#ifdef _DEBUG
		Serial.println("StartSendingSensorDataReceive");
		Serial.print("tag: ");
		Serial.println(tag);
		Serial.print("interval: ");
		Serial.println(interval);
#endif

		main->AddTask(new SendSensorDataTask_t(tag, main->_sensor_manager, main->_communicator, interval));

		return 0;
	}, this);

	_communicator->SetOnStartBlutoothReadingReceive([](void* data, unsigned int tag) -> int {
		auto main = static_cast<StdMain_t*>(data);

#ifdef _DEBUG
		Serial.println("StartBlutoothReadingReceive");
		Serial.print("tag: ");
		Serial.println(tag);
#endif

		main->AddTask(new ReceiveBluetoothMessageTask_t(tag));

		return 0;
	}, this);

	_communicator->SetOnGetTaskStateReceive([](void* data, int worker_id) -> int {
		auto main = static_cast<StdMain_t*>(data);

#ifdef _DEBUG
		Serial.println("GetTaskStateReceive");
		Serial.print("worker id: ");
		Serial.println(worker_id);
#endif

		if (main->SendWorkerState(worker_id)) {
			ThrowException(Exceptions::EC_SM_WRONG_WORKER_ID);
			return 1;
		}

		return 0;
	}, this);
}

int StdMain_t::Begin() {
	return _communicator->Begin();
}

int StdMain_t::SendWorkerState(int worker_id) const {

#ifdef _DEBUG
	Serial.print("Sending state of worker, id: ");
	Serial.println(worker_id);
#endif

	WORKER_STATUS worker_status;
	TaskState_t* task_state_ptr;

	_task_pool.GetWorkerState(worker_id, task_state_ptr, worker_status);

	return _communicator->SendWorkerState(task_state_ptr, worker_id, worker_status);
}

int StdMain_t::AddTask(Task_t* task) {
	if (_task_pool.AddTask(task)) {
		if (_communicator->SendException(Exceptions::GetLastException())) {
			return 1;
		}
		Exceptions::Release();
	} else {
		TaskState_t* task_state_ptr;
		WORKER_STATUS worker_status;
		int worker_id;
		_task_pool.GetLastAddedWorkerState(task_state_ptr, worker_id, worker_status);
		return _communicator->SendWorkerState(task_state_ptr, worker_id, worker_status);
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

#include "StdMain.h"
#include "MathConstants.h"
#include "BlinkFlashLightTask.h"
#include "SetFlashlightStateTask.h"
#include "SendSensorDataTask.h"
#include "ReceiveBluetoothMessageTask.h"
#include "I2CScanTask.h"

StdMain_t::StdMain_t(Communicator_t* communicator, Movement_t* movement,
	SensorManager_t* sensor_manager, PeripheryManager_t* periphery_manager)
	: _task_pool(3) {
	_communicator = communicator;
	_movement = movement;
	_sensor_manager = sensor_manager;
	_periphery_manager = periphery_manager;

	_communicator->SetOnSetFlashlightStateReceive([](void* data, int worker_id, unsigned int tag, byte state) -> int {
		auto main = static_cast<StdMain_t*>(data);

		LOGLN("SetOnSetFlashlightStateReceive");
		LOG("worker_id: ");
		LOGLN(worker_id);
		LOG("tag: ");
		LOGLN(tag);
		LOG("state: ");
		LOGLN(state);

		return main->AddTask(new SetFlashlightStateTask_t(tag, state, main->_periphery_manager), worker_id);
	}, this);

	_communicator->SetOnBlinkFlashlightReceive([](void* data, int worker_id, unsigned int tag, unsigned int count) -> int {
		auto main = static_cast<StdMain_t*>(data);

		LOGLN("SetOnBlinkFlashlightReceive");
		LOG("worker_id: ");
		LOGLN(worker_id);
		LOG("tag: ");
		LOGLN(tag);
		LOG("count: ");
		LOGLN(count);

		return main->AddTask(new BlinkFlashlightTask_t(tag, count, main->_periphery_manager), worker_id);
	}, this);

	_communicator->SetOnFreeWorkerReceive([](void* data, int worker_id) -> int {
		auto main = static_cast<StdMain_t*>(data);

		LOGLN("SetOnCancelTaskReceive");
		LOG("worker id: ");
		LOGLN(worker_id);

		return main->_task_pool.FreeWorker(worker_id);
	}, this);

	_communicator->SetOnSetMotorsThrustReceive([](void* data, int motor1Thrust, int motor2Thrust, int motor3Thrust, 
		int motor4Thrust, int motor5Thrust, int motor6Thrust) -> int {
		auto main = static_cast<StdMain_t*>(data);

		Serial.println("SetMotorsThrustReceive");
		LOG("motor1: ");
		LOGLN(motor1Thrust);
		LOG("motor2: ");
		LOGLN(motor2Thrust);
		LOG("motor3: ");
		LOGLN(motor3Thrust);
		LOG("motor4: ");
		LOGLN(motor4Thrust);
		LOG("motor5: ");
		LOGLN(motor5Thrust);
		LOG("motor6: ");
		LOGLN(motor6Thrust);

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

		LOGLN("SetManipulatorPositionReceive");
		LOG("handPosition: ");
		LOGLN(handPosition);
		LOG("armPosition: ");
		LOGLN(armPosition);

		main->_periphery_manager->GetManipulator()->SetHandAngle(constrain(handPosition, 0, 4095) / 4095.f * PI2);
		main->_periphery_manager->GetManipulator()->SetArmAngle(constrain(armPosition, 0, 4095) / 4095.f * PI2);

		return 0;
	}, this);

	_communicator->SetOnStartSendingSensorDataReceive([](void* data, int worker_id, unsigned int tag, unsigned int interval) -> int {
		auto main = static_cast<StdMain_t*>(data);

		LOGLN("StartSendingSensorDataReceive");
		LOG("worker_id: ");
		LOGLN(worker_id);
		LOG("tag: ");
		LOGLN(tag);
		LOG("interval: ");
		LOGLN(interval);

		main->AddTask(new SendSensorDataTask_t(tag, main->_sensor_manager, main->_communicator, interval), worker_id);

		return 0;
	}, this);

	_communicator->SetOnStartBlutoothReadingReceive([](void* data, int worker_id, unsigned int tag) -> int {
		auto main = static_cast<StdMain_t*>(data);

		LOGLN("StartBlutoothReadingReceive");
		LOG("worker_id: ");
		LOGLN(worker_id);
		LOG("tag: ");
		LOGLN(tag);

		main->AddTask(new ReceiveBluetoothMessageTask_t(tag), worker_id);

		return 0;
	}, this);

	_communicator->SetOnGetTaskStateReceive([](void* data, int worker_id) -> int {
		auto main = static_cast<StdMain_t*>(data);

		LOGLN("GetTaskStateReceive");
		LOG("worker id: ");
		LOGLN(worker_id);

		if (main->SendWorkerState(worker_id)) {
			ThrowException(Exceptions::EC_SM_WRONG_WORKER_ID);
			return 1;
		}

		return 0;
	}, this);

	_communicator->SetOnI2CScanReceive([](void* data, int worker_id, unsigned int tag) -> int {
		auto main = static_cast<StdMain_t*>(data);

		LOGLN("I2CScanReceive");
		LOG("worker id: ");
		LOGLN(worker_id);
		LOG("tag: ");
		LOGLN(tag);

		main->AddTask(new I2CScanTask_t(tag), worker_id);

		return 0;
	}, this);

	_communicator->SetOnGetLastUsedWorkerStateReceive([](void* data) -> int {
		auto main = static_cast<StdMain_t*>(data);

		LOGLN("GetLastUsedWorkerStateReceive");

		main->SendLastUsedWorkerState();

		return 0;
	}, this);

	_communicator->SetOnSetLocalForceReceive([](void* data, int move_x, int move_y, int move_z, int rotate_y, int rotate_z) -> int {
		auto main = static_cast<StdMain_t*>(data);

		LOGLN("SetLocalForceReceive");
		LOG("move_x: ");
		LOGLN(move_x);
		LOG("move_y: ");
		LOGLN(move_y);
		LOG("move_y: ");
		LOGLN(move_y);
		LOG("move_z: ");
		LOGLN(move_z);
		LOG("rotate_y: ");
		LOGLN(rotate_y);
		LOG("rotate_z: ");
		LOGLN(rotate_z);

		main->_movement->SetLocalMovementForce(move_x / 32768.f, move_y / 32768.f, move_z / 32768.f);
		main->_movement->SetLocalRotateForce(rotate_y / 32768.f, rotate_z / 32768.f);

		return 0;
	}, this);

	_communicator->SetOnPingReceive([](void* data) -> int {
		auto main = static_cast<StdMain_t*>(data);

		LOGLN("PingReceive");

		return main->_communicator->SendPong();
	}, this);
}

int StdMain_t::Begin() {
	return _communicator->Begin();
}

int StdMain_t::SendWorkerState(int worker_id) const {
	LOG("Sending state of worker, id: ");
	LOGLN(worker_id);

	WORKER_STATUS worker_status;
	TaskState_t* task_state_ptr;

	_task_pool.GetWorkerState(worker_id, task_state_ptr, worker_status);

	return _communicator->SendWorkerState(task_state_ptr, worker_id, worker_status);
}

int StdMain_t::SendLastUsedWorkerState() const {
	LOG("Sending state of last used worker, id: ");
	LOGLN(_task_pool.GetLastAddedTaskWorkerId());

	WORKER_STATUS worker_status;
	TaskState_t* task_state_ptr;
	int worker_id;

	if (_task_pool.GetLastAddedWorkerState(task_state_ptr, worker_id, worker_status)) {
		if (Exceptions::GetLastCode() == Exceptions::EC_TP_NO_ONE_WORKER_IS_USED) {
			Exceptions::Release();
			return _communicator->SendLastUsedWorkerState(nullptr, -1, WS_BUSY);
		}
		return 1;
	}

	return _communicator->SendLastUsedWorkerState(task_state_ptr, worker_id, worker_status);
}

int StdMain_t::AddTask(Task_t* task, int worker_id = -1) {
	if (_task_pool.AddTask(task, worker_id)) {
		if (_communicator->SendException(Exceptions::GetLastException())) {
			return 1;
		}
		Exceptions::Release();
	} else {
		SendLastUsedWorkerState();
	}
	return 0;
}

void StdMain_t::Loop() {
	if (_communicator->Update()) {
		_communicator->SendException(Exceptions::GetLastException());
		Exceptions::Release();
	}
	_task_pool.Update();
	_movement->Update();
}

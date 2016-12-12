#include "Main.h"

Main_t::Main_t(Communicator_t* communicator, Movement_t* movement, 
	SensorManager_t* sensor_manager, PeripheryManager_t* periphery_manager) 
	: _task_pool(3)
{
	_communicator = communicator;
	_movement = movement;
	_sensor_manager = sensor_manager;
	_periphery_manager = periphery_manager;

	message_receivers[MT_SET_MOTOR_THRUST] = &RecieveSetMotorThrustMessage;
	message_receivers[MT_SET_POSITION] = &RecieveSetPositionMessage;
	message_receivers[MT_FREE_WORKER] = &RecieveFreeWorkerMessage;
	message_receivers[MT_SET_FLASHLIGHT_STATE] = &RecieveSetFlashlightMessage;
	message_receivers[MT_BLINK_FLASHLIGHT] = &RecieveBlinkFlashlightMessage;
}

void Main_t::SendTaskState(unsigned int worker_id) const {

#ifdef _DEBUG
	Serial.print("Sending state of worker, id: ");
	Serial.println(worker_id);
#endif

	TaskStateUnion_t task_state_union;
	_task_pool.GetTaskState(worker_id, task_state_union);

	TaskStateMessage_t send_message(task_state_union);
	_communicator->SendMessage(&send_message);
}

void Main_t::RecieveSetMotorThrustMessage(Main_t* _this, const MessageUnion_t& message) {
	auto m = &message.set_motor_thrust_message;

#ifdef _DEBUG
	Serial.println("RecieveSetMotorThrustMessage");
	Serial.print("Message type: ");
	Serial.println(int(m->message_type));
	Serial.print("Thrust: ");
	Serial.println(m->thrust);
#endif

	_this->_movement->SetMotorThrust(0, m->thrust * 1.0f / 400);
}

void Main_t::RecieveSetPositionMessage(Main_t* _this, const MessageUnion_t& message) {
	auto m = &message.set_position_message;

	_this->_movement->SetDepth(m->position);
}

void Main_t::RecieveFreeWorkerMessage(Main_t* _this, const MessageUnion_t& message) {
	auto m = &message.free_worker_message;

#ifdef _DEBUG
	Serial.println("RecieveFreeWorkerMessage");
	Serial.print("Message type: ");
	Serial.println(int(m->message_type));
	Serial.print("Worker id: ");
	Serial.println(m->worker_id);
#endif

	if (_this->_task_pool.FreeWorker(m->worker_id)) {
		_this->SendTaskState(m->worker_id);
	}
}

void Main_t::RecieveSetFlashlightMessage(Main_t* _this, const MessageUnion_t& message) {
	auto m = &message.set_flashlight_state_message;

#ifdef _DEBUG
	Serial.println("RecieveSetFlashlightMessage");
	Serial.print("Message type: ");
	Serial.println(int(m->message_type));
	Serial.print("Task id: ");
	Serial.println(m->task_id);
	Serial.print("State: ");
	Serial.println(int(m->state));
#endif

	_this->_task_pool.AddTask(new SetFlashlightStateTask_t(m->task_id, m->state, _this->_periphery_manager));
	_this->SendTaskState(_this->_task_pool.GetLastAddedTaskWorkerId());
}

void Main_t::RecieveBlinkFlashlightMessage(Main_t* _this, const MessageUnion_t& message) {
	auto m = &message.blink_flashlight_message;

#ifdef _DEBUG
	Serial.println("RecieveBlinkFlashlightMessage");
	Serial.print("Message type: ");
	Serial.println(int(m->message_type));
	Serial.print("Id: ");
	Serial.println(m->task_id);
	Serial.print("Count: ");
	Serial.println(int(m->count));
#endif

	_this->_task_pool.AddTask(new BlinkFlashlightTask_t(m->task_id, m->count, _this->_periphery_manager));
	_this->SendTaskState(_this->_task_pool.GetLastAddedTaskWorkerId());
}

void Main_t::Loop() {
	MessageUnion_t message_union;
	while(_communicator->ReceiveMessage(message_union)) {
		message_receivers[message_union.base_message.message_type](this, message_union);
	}

	_task_pool.Update();
}

DebugMotorsMain_t::DebugMotorsMain_t(Motor_t* motor) {
	_motor = motor;
}

void DebugMotorsMain_t::Loop() {
	_motor->SetThrust(1);
	delay(2000);
	_motor->SetThrust(-1);
	delay(2000);
}

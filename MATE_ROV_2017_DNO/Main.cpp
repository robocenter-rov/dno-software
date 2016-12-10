#include "Main.h"

Main_t::Main_t(Communicator_t* communicator, Movement_t* movement, 
	SensorManager_t* sensor_manager, PeripheryManager_t* periphery_manager) 
{
	_communicator = communicator;
	_movement = movement;
	_sensor_manager = sensor_manager;
	_periphery_manager = periphery_manager;

	message_receivers[SET_POSITION_MT] = &RecieveSetPositionMessage;
	message_receivers[SET_FLASHLIGHT_STATE_MT] = &RecieveSetFlashlightMessage;
}

void Main_t::RecieveSetPositionMessage(Main_t* _this, Message_t* message) {
	auto m = static_cast<SetPositionMessage_t*>(message);

	_this->_movement->SetDepth(m->position);
}

void Main_t::RecieveSetFlashlightMessage(Main_t* _this, Message_t* message) {
	auto m = static_cast<SetFlashlightStateMessage_t*>(message);
	_this->_periphery_manager->SetFlashlightState(m->state);
}

void Main_t::Loop() {
	Message_t* message;
	while(message = _communicator->ReceiveMessage()) {
		message_receivers[message->message_type](this, message);
	}
}

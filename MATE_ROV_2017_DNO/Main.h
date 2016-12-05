#pragma once
#include "Communicator.h"
#include "Movement.h"
#include "Message.h"

class Main_t {
private:
	Communicator_t* _communicator;
	Movement_t* _movement;
	SensorManager_t* _sensor_manager;

	typedef void (*message_receiver_t)(Main_t* _this, Message_t* message);

	static void RecieveSetPositionMessage(Main_t* _this, Message_t* message);

	message_receiver_t message_receivers[MAX_MESSAGE_TYPE_ID];
public:
	Main_t(Communicator_t* communicator, Movement_t* movement, SensorManager_t* sensor_manager);

	void Loop();
};

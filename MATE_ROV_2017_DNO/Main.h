#pragma once
#include "Communicator.h"
#include "Movement.h"
#include "PeripheryManager.h"
#include "Message.h"
#include "TaskPool.h"

class BaseMain_t {
public:
	virtual void Loop() {}
	virtual ~BaseMain_t() {}
};

class Main_t : public BaseMain_t {
private:
	Communicator_t* _communicator;
	Movement_t* _movement;
	SensorManager_t* _sensor_manager;
	PeripheryManager_t* _periphery_manager;
	TaskPool_t _task_pool;

	typedef void (*message_receiver_t)(Main_t* _this, const MessageUnion_t& message);

	void SendTaskState(unsigned int worker_id) const;

	static void RecieveSetPositionMessage(Main_t* _this, const MessageUnion_t& message);

	static void RecieveFreeWorkerMessage(Main_t* _this, const MessageUnion_t& message);

	static void RecieveSetFlashlightMessage(Main_t* _this, const MessageUnion_t& message);
	static void RecieveBlinkFlashlightMessage(Main_t* _this, const MessageUnion_t& message);

	message_receiver_t message_receivers[MT_MAX_MESSAGE_TYPE_ID];
public:
	Main_t(Communicator_t* communicator, Movement_t* movement, 
		SensorManager_t* sensor_manager, PeripheryManager_t* periphery_manager);

	void Loop() override;
};

class DebugMotorsMain_t : public BaseMain_t {
private:
	Motor_t* _motor;
public:
	DebugMotorsMain_t(Motor_t* motor);

	void Loop() override;

};

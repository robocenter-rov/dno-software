#pragma once

#include "Main.h"
#include "Communicator.h"
#include "Movement.h"
#include "PeripheryManager.h"
#include "TaskPool.h"

class StdMain_t : public Main_t {
private:
	Communicator_t* _communicator;
	Movement_t* _movement;
	SensorManager_t* _sensor_manager;
	PeripheryManager_t* _periphery_manager;
	TaskPool_t _task_pool;

	void SendTaskState(unsigned int worker_id) const;
	int AddTask(Task_t* task);
public:
	StdMain_t(Communicator_t* communicator, Movement_t* movement,
		SensorManager_t* sensor_manager, PeripheryManager_t* periphery_manager);

	int Begin() override;
	void Loop() override;
};

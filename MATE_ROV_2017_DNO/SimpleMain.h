#pragma once

#include "Main.h"
#include "SimpleCommunicator.h"


class SimpleMain_t : public Main_t
{
private:
	SimpleCommunicator_t* _communicator;
	Movement_t* _movement;
	SensorManager_t* _sensor_manager;
	PeripheryManager_t* _periphery_manager;

public:
	SimpleMain_t(SimpleCommunicator_t* communicator, Movement_t* movement,
		SensorManager_t* sensor_manager, PeripheryManager_t* periphery_manager);
	~SimpleMain_t();
	int Begin() override;
	void Loop() override;
};


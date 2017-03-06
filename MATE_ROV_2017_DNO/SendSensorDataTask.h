#pragma once

#include "Task.h"
#include "SensorManager.h"
#include "Communicator.h"

class SendSensorDataTask_t : public Task_t {
private:
	SensorManager_t* _sensor_manager;
	Communicator_t* _communicator;
	unsigned int _interval;

	unsigned int _last_send_time;
public:
	SendSensorDataTask_t(int id, SensorManager_t* sensor_manager, Communicator_t* communicator, unsigned int interval);
	bool LockNeededResources(RESOURCE& out_locked_resource, int worker_id) override;
	bool UpdateState(SelfExpandoContainer_t<TaskState_t>& out_state) override;
};

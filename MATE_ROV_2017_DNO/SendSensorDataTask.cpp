#include "SendSensorDataTask.h"

SendSensorDataTask_t::SendSensorDataTask_t(int id, SensorManager_t* sensor_manager, Communicator_t* communicator, unsigned int interval) 
	: Task_t(id, TI_SEND_SENSOR_DATA) {
	_sensor_manager = sensor_manager;
	_communicator = communicator;
	_interval = interval;
	_last_send_time = 0;
}

bool SendSensorDataTask_t::LockNeededResources(RESOURCE& out_locked_resource, int worker_id) {
	return true;
}

bool SendSensorDataTask_t::UpdateState(SelfExpandoContainer_t<TaskState_t>& out_state) {
	if (millis() - _last_send_time > _interval) {
		SensorData_t t = _sensor_manager->GetSensorData();
		_communicator->SendSensorData(&t);
		out_state = SendSensorDataTaskState_t(_tag);
		_last_send_time = millis();
	}

	return false;
}

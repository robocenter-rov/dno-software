#include "SensorManager.h"

SensorManager_t::SensorManager_t(SensorRotation_t* sensor_rotation, SensorDepth_t* sensor_depth) {
	_sensorRotation = sensor_rotation;
	_sensorDepth = sensor_depth;
}

float SensorManager_t::GetDepth() const {
	return _sensorDepth->GetDepth();
}

SensorRotation_t::Data_t SensorManager_t::GetRotation() const {
	return _sensorRotation->GetRotation();
}

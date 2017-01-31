#pragma once
#include "SensorData.h"
#include "SensorDepth.h"
#include "SensorRotation.h"

class SensorManager_t {
private:
	SensorRotation_t* _sensorRotation;
	SensorDepth_t* _sensorDepth;
public:
	SensorManager_t(SensorRotation_t* sensor_rotation, SensorDepth_t* sensor_depth);

	float GetDepth() const;
	SensorRotation_t::Data_t GetRotation() const;

	void Update();
};

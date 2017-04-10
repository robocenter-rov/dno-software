#pragma once
#include "SensorDepth.h"
#include "SensorRotation.h"

class SensorManager_t {
private:
	SensorRotation_t* _sensorRotation;
	SensorDepth_t* _sensorDepth;
public:
	SensorManager_t(SensorRotation_t* sensor_rotation, SensorDepth_t* sensor_depth);

	void GetRotation(float& q1, float& q2, float& q3, float& q4);
	void GetRawRotation(int* raw_data);
	void GetCalibratedRotation(float* calibrated_data);

	float GetDepth();
	float GetRawDepth();

	void Update();
};

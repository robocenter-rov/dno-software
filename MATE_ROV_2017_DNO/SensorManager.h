#pragma once
#include "SensorDepth.h"
#include "SensorRotation.h"

class SensorManager_t {
private:
	SensorDepth_t* SensorDepth;
	SensorRotation_t* SensorRotation;
public:
	SensorManager_t();

	float GetDepth() const;
	SensorRotation_t::Data_t GetRotation() const;

	void Update();
};

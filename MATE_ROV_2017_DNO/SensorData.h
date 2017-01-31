#pragma once
#include "SensorRotation.h"

struct SensorData_t {
	SensorRotation_t::Data_t rotationData;
	float depthData;
};

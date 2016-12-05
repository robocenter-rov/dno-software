#include "SensorRotation.h"

SensorRotation_t::SensorRotation_t() {}

SensorRotation_t::Data_t SensorRotation_t::GetRotation() {
	return Data_t(0, 0, 0, 0, 0, 0);
}

SensorRotation_t::~SensorRotation_t() {}

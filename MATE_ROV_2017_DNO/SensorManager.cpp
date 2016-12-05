#include "SensorManager.h"

SensorManager_t::SensorManager_t() {

}

float SensorManager_t::GetDepth() const {
	return SensorDepth->GetDepth();
}

SensorRotation_t::Data_t SensorManager_t::GetRotation() const {
	return SensorRotation->GetRotation();
}

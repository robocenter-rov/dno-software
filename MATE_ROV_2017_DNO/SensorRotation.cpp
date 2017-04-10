#include "SensorRotation.h"
#include "Arduino.h"

SensorRotation_t::SensorRotation_t() {}

void SensorRotation_t::GetRotation(float& q1, float& q2, float& q3, float& q4) {
	q1 = 1;
	q2 = 0;
	q3 = 0;
	q4 = 0;
}

void SensorRotation_t::GetRawData(int* raw_data) {
	memset(raw_data, 0, sizeof(int) * 9);
}

void SensorRotation_t::GetCalibratedData(float* calibrated_data) {
	memset(calibrated_data, 0, sizeof(float) * 9);
}

SensorRotation_t::~SensorRotation_t() {}

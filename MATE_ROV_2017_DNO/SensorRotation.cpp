#include "SensorRotation.h"
#include "Arduino.h"

SensorRotation_t::SensorRotation_t()
{
	_nineDOF = FreeIMU();
	delay(5);
	_nineDOF.init();
	delay(5);
}

void SensorRotation_t::GetRotation(float& q1, float& q2, float& q3, float& q4) {
	q1 = 1;
	q2 = 0;
	q3 = 0;
	q4 = 0;
}

void SensorRotation_t::GetRawData(int* raw_data) {
	//memset(raw_data, 0, sizeof(int) * 9);
	_nineDOF.getRawValues(raw_data);
}

void SensorRotation_t::GetCalibratedData(float* calibrated_data) {
	_nineDOF.getValues(calibrated_data);
}

SensorRotation_t::~SensorRotation_t() {}

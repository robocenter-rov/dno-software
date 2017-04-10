#pragma once
#include "Libs/FreeIMU/FreeIMU.h"

class SensorRotation_t {
private:
	FreeIMU _nineDOF;
public:
	SensorRotation_t();

	void GetRotation(float& q1, float& q2, float& q3, float& q4);
	void GetRawData(int* raw_data);
	void GetCalibratedData(float* calibrated_data);

	~SensorRotation_t();
};


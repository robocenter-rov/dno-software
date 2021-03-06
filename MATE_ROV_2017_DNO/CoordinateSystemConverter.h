#pragma once

#include "SensorManager.h"
#include "Quaternions.h"

class CoordinateSystemConverter_t {
private:
	SensorManager_t* _sensor_manager;
public:
	CoordinateSystemConverter_t(SensorManager_t* sensor_manager) {
		_sensor_manager = sensor_manager;
	}

	void local_to_global(float in_x, float in_y, float in_z, float& out_x, float& out_y, float& out_z) {
		float q0;
		float q1;
		float q2;
		float q3;

		_sensor_manager->GetRotation(q0, q1, q2, q3);

		//TODO:Implement
	}

	void GlobalToLocal(float in_x, float in_y, float in_z, float& out_x, float& out_y, float& out_z) const {
		float q0;
		float q1;
		float q2;
		float q3;

		_sensor_manager->GetRotation(q0, q1, q2, q3);

		quatInvert(q0, q1, q2, q3, q0, q1, q2, q3);
		quatTransformVector(q0, q1, q2, q3, in_x, in_y, in_z, out_x, out_y, out_z);
	}
};

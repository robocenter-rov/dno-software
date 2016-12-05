#pragma once

#include "SensorManager.h"

class CoordinateSystemConverter_t {
private:
	SensorManager_t* _sensor_manager;
public:
	CoordinateSystemConverter_t(SensorManager_t* sensor_manager) {
		_sensor_manager = sensor_manager;
	}

	void local_to_global(float in_x, float in_y, float in_z, float& out_x, float& out_y, float& out_z) {
		auto sensor_data = _sensor_manager->GetRotation();

		float x = sensor_data.RotationX;
		float y = sensor_data.RotationY;
		float z = sensor_data.RotationZ;

	}

	void glogal_to_local(float in_x, float in_y, float in_z, float& out_x, float& out_y, float& out_z) {
		auto sensor_data = _sensor_manager->GetRotation();

		float x = sensor_data.RotationX;
		float y = sensor_data.RotationY;
		float z = sensor_data.RotationZ;

	}
};
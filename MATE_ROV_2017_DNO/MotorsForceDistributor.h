#pragma once
#include "LocalMotorsForceDistributor.h"
#include "SensorManager.h"
#include "CoordinateSystemConverter.h"

class MotorsForceDistributor_t {
private:
	LocalMotorsForceDistributor_t _local_force_distrib;
	CoordinateSystemConverter_t _coordinate_system_converter;
public:
	MotorsForceDistributor_t(SensorManager_t* sensor_manager, Motors_t* motors) :
	_local_force_distrib(motors), _coordinate_system_converter(sensor_manager) {}

	void AddLocalMoveForce(float x, float y, float z) {
		_local_force_distrib.AddMoveForce(x, y, z);
	}

	void SetLocalMoveForce(float x, float y, float z) {
		_local_force_distrib.SetMoveForce(x, y, z);
	}

	void AddGlobalMoveForce(float x, float y, float z) {
		float local_x, local_y, local_z;

		_coordinate_system_converter.GlobalToLocal(x, y, z, local_x, local_y, local_z);

		AddLocalMoveForce(local_x, local_y, local_z);
	}

	void SetGlobalMoveForce(float x, float y, float z) {
		float local_x, local_y, local_z;

		_coordinate_system_converter.GlobalToLocal(x, y, z, local_x, local_y, local_z);

		SetLocalMoveForce(local_x, local_y, local_z);
	}

	void AddLocalRotateForce(float y, float z) {
		_local_force_distrib.AddRotateForce(y, z);
	}

	void SetLocalRotateForce(float y, float z) {
		_local_force_distrib.SetRotateForce(y, z);
	}
};

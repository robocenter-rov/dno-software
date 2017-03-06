#pragma once
#include "DepthRegulator.h"
#include "RotationRegulator.h"
#include "PositionRegulator.h"

class Regulators_t {
private:
	AutoPosition_t _position_regulator;
	AutoRotation_t _rotation_regulator;
public:
	Regulators_t(MotorsForceDistributor_t* force_distrib, SensorManager_t* sensor_manager,
		PidCoefficient_t depth_pid, PidCoefficient_t velocity_pid,
			PidCoefficient_t y_cource_pid, PidCoefficient_t z_course_pid,
				PidCoefficient_t y_algular_velocity_pid, PidCoefficient_t z_angular_velocity_pid
			) : 
	_position_regulator(force_distrib, sensor_manager, depth_pid, velocity_pid),
	_rotation_regulator(force_distrib, sensor_manager, y_cource_pid, z_course_pid, y_algular_velocity_pid, z_angular_velocity_pid){}

	void SetTargetDepth(float depth) {
		_position_regulator.SetDepth(depth);
	}

	void SetVelocity(float x, float y, float z) {
		_position_regulator.SetVelocity(x, y, z);
	}

	void SetRotation(float y, float z) {
		_rotation_regulator.SetRotate(y, z);
	}

	void SetAngularVelocity(float y, float z) {
		_rotation_regulator.SetAngularVelocity(y, z);
	}

	void Update() {
		_position_regulator.Update();
		_rotation_regulator.Update();
	}
};


#pragma once
#include "DepthRegulator.h"
#include "VelocityRegulator.h"

class AutoPosition_t {
private:
	AutoDepth_t _depth_regulator;
	AutoVelocity_t _velocity_regulator;

	bool _use_depth_regulator;
public:
	AutoPosition_t(MotorsForceDistributor_t* force_distrib, SensorManager_t* sensor_manager,
		PidCoefficient_t depth_pid, PidCoefficient_t velocity_pid) :
		_depth_regulator(sensor_manager, depth_pid),
		_velocity_regulator(force_distrib, sensor_manager, velocity_pid) {}

	void SetDepth(float depth) {
		_depth_regulator.SetDepth(depth);
		_use_depth_regulator = true;
	}

	void SetVelocity(float x, float y, float z) {
		_velocity_regulator.SetVelocity(x, y, z);
		_use_depth_regulator = false;
	}

	void Update() {
		if (_use_depth_regulator) {
			_depth_regulator.Update();
		} else {
			_velocity_regulator.Update();
		}
	}
};

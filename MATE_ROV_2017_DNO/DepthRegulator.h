#pragma once

#include "SensorManager.h"
#include "MotorsForceDistributor.h"
#include "MovementRegulator.h"

class AutoDepth_t : public MovementRegulator_t {
public:
	AutoDepth_t(SensorManager_t* sensor_manager, float p = 0, float i = 0, float d = 0) : MovementRegulator_t(sensor_manager, p, i, d) {
		_pid.SetOutputLimits(-2, 2);
	}

	void Update(MotorsForceDistributor_t* motors_force_distributor) {
		_input = _sensor_manager->GetDepth();
		_pid.Compute();
		motors_force_distributor->AddGlobalMoveForce(0, 0, _output);
	}

	void SetDepth(float target_depth) {
		_target = target_depth;
	}

	void SetTunings(float Kp, float Ki, float Kd) {
		_pid.SetTunings(Kp, Ki, Kd);
	}
};

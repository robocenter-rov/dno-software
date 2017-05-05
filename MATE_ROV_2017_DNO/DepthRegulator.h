#pragma once

#include "SensorManager.h"
#include "MotorsForceDistributor.h"
#include "MovementRegulator.h"

class AutoDepth_t : public MovementRegulator_t {
public:
	using MovementRegulator_t::MovementRegulator_t;

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

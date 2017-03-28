#pragma once

#include "SensorManager.h"
#include "Libs/PID/PID_v1.h"
#include "PidCoefficients.h"
#include "MotorsForceDistributor.h"

class AutoDepth_t {
private:
	MotorsForceDistributor_t* _force_distrib;
	SensorManager_t* _sensor_manager;

	PID _pid;

	double _pid_input;
	double _pid_output;
	double _target_depth;
public:
	AutoDepth_t(MotorsForceDistributor_t* force_distrib, SensorManager_t* sensor_manager,
		PidCoefficient_t pid_coeff) : _pid(&_pid_input, &_pid_output, &_target_depth, pid_coeff.D, pid_coeff.I, pid_coeff.D, DIRECT)
	{
		_force_distrib = force_distrib;
		_sensor_manager = sensor_manager;
		_target_depth = 0;
	}

	void Update() {
		_pid_input = _sensor_manager->GetDepth();
		_pid.Compute();
		_force_distrib->AddGlobalMoveForce(0, 0, _pid_output);
	}

	void SetDepth(float target_depth) {
		_target_depth = target_depth;
	}

	void SetTunings(float Kp, float Ki, float Kd) {
		_pid.SetTunings(Kp, Ki, Kd);
	}
};

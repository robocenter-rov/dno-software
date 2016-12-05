#pragma once
#include "MotorsForceDistributor.h"
#include "SensorManager.h"
#include <PID_v1.h>
#include "PidCoefficients.h"

class AutoVelocity_t {
	MotorsForceDistributor_t* _force_distrib;
	SensorManager_t* _sensor_manager;

	PID _pid;

	double _pid_input;
	double _pid_output;
	double _target_x_velocity;
	double _target_z_velocity;
	double _target_y_velocity;
public:
	AutoVelocity_t(MotorsForceDistributor_t* force_distrib, SensorManager_t* sensor_manager,
		PidCoefficient_t pid_coeff) : _pid(&_pid_input, &_pid_output, &_target_z_velocity, pid_coeff.D, pid_coeff.I, pid_coeff.D, DIRECT) 
	{
		_force_distrib = force_distrib;
		_sensor_manager = sensor_manager;
		_target_x_velocity = 0;
		_target_y_velocity = 0;
		_target_z_velocity = 0;
	}

	void Update() {
		_pid_input = _sensor_manager->GetDepth();
		_pid.Compute();
		_force_distrib->AddLocalMoveForce(_target_x_velocity, _target_y_velocity, _pid_output);
	}

	void SetVelocity(float x, float y, float z) {
		_target_x_velocity = x;
		_target_y_velocity = y;
		_target_z_velocity = z;
	}

	void SetTunings(float Kp, float Ki, float Kd) {
		_pid.SetTunings(Kp, Ki, Kd);
	}
};

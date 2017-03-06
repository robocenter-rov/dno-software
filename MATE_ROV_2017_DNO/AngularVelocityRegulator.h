#pragma once
#include "MotorsForceDistributor.h"
#include "SensorManager.h"
#include <PID_v1.h>
#include "PidCoefficients.h"

class AutoAngularVelocity_t {
private:
	MotorsForceDistributor_t* _force_distrib;
	SensorManager_t* _sensor_manager;
	
	PID _z_pid;
	PID _y_pid;

	double _y_pid_input;
	double _y_pid_output;
	double _z_pid_input;
	double _z_pid_output;
	double _target_y_angular_velocity;
	double _target_z_angular_velocity;
public:
	AutoAngularVelocity_t(MotorsForceDistributor_t* force_distrib, SensorManager_t* sensor_manager,
		PidCoefficient_t y_pid_coeff, PidCoefficient_t z_pid_coeff) :
		_y_pid(&_y_pid_input, &_y_pid_output, &_target_y_angular_velocity, y_pid_coeff.P, y_pid_coeff.I, y_pid_coeff.D, DIRECT),
		_z_pid(&_z_pid_input, &_z_pid_output, &_target_z_angular_velocity, z_pid_coeff.P, z_pid_coeff.I, z_pid_coeff.D, DIRECT)
	{
		_force_distrib = force_distrib;
		_sensor_manager = sensor_manager;
		_target_y_angular_velocity = 0;
		_target_z_angular_velocity = 0;
	}

	void Update() {
		SensorRotation_t::Data_t rotation = _sensor_manager->GetRotation();

		_y_pid_input = rotation.AngleVelocityY;
		_y_pid.Compute();
		_force_distrib->AddLocalRotateForce(_y_pid_output, 0);

		_z_pid_input = rotation.AngleVelocityZ;
		_z_pid.Compute();
		_force_distrib->AddLocalRotateForce(0, _z_pid_output);
	}

	void SetAngularVelocity(float y, float z) {
		_target_y_angular_velocity = y;
		_target_z_angular_velocity = z;
	}
};

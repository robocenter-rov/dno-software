#pragma once
#include "MotorsForceDistributor.h"
#include "SensorManager.h"
#include "Libs/PID/PID_v1.h"
#include "PidCoefficients.h"

class AutoCourse_t {
private:
	MotorsForceDistributor_t* _force_distrib;
	SensorManager_t* _sensor_manager;

	PID _y_pid;
	PID _z_pid;

	double _y_pid_input;
	double _y_pid_output;
	double _z_pid_input;
	double _z_pid_output;
	double _target_y_cource;
	double _target_z_cource;
public:
	AutoCourse_t(MotorsForceDistributor_t* force_distrib, SensorManager_t* sensor_manager,
		PidCoefficient_t y_pid_coeff, PidCoefficient_t z_pid_coeff) :
		_y_pid(&_y_pid_input, &_y_pid_output, &_target_y_cource, y_pid_coeff.P, y_pid_coeff.I, y_pid_coeff.D, DIRECT),
		_z_pid(&_z_pid_input, &_z_pid_output, &_target_z_cource, z_pid_coeff.P, z_pid_coeff.I, z_pid_coeff.D, DIRECT)
	{
		_force_distrib = force_distrib;
		_sensor_manager = sensor_manager;
		_target_y_cource = 0;
		_target_z_cource = 0;
	}

	void Update() {
		float q0, q1, q2, q3;
		_sensor_manager->GetRotation(q0, q1, q2, q3);

		float y, p, r;
		quatToYpr(q0, q1, q2, q3, y, p, r);

		_y_pid_input = p;
		_y_pid.Compute();
		_force_distrib->AddLocalRotateForce(_y_pid_output, 0);

		_z_pid_input = y;
		_z_pid.Compute();
		_force_distrib->AddLocalRotateForce(0, _z_pid_output);
	}

	void SetRotate(float y, float z) {
		_target_y_cource = y;
		_target_z_cource = z;
	}
};

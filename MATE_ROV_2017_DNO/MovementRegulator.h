#pragma once

#include "SensorManager.h"
#include "Libs/PID/PID_v1.h"

class MovementRegulator_t {
protected:
	SensorManager_t* _sensor_manager;

	double _input;
	double _output;
	double _target;

	PID _pid;
public:
	MovementRegulator_t(SensorManager_t* sensor_manager, float p = 0, float i = 0, float d = 0) 
		:  _sensor_manager(sensor_manager), _input(0), _output(0), _target(0), _pid(&_input, &_output, &_target, p, i, d, 1) {}

	void SetTarget(float target) {
		_target = target;
	}

	void SetPid(float p, float i, float d) {
		_pid.SetTunings(p, i, d);
	}
};
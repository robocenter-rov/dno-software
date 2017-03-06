#pragma once
#include "CourseRegulator.h"
#include "AngularVelocityRegulator.h"
#include "../Movement/MotorsForceDistributor.h"

class AutoRotation_t {
private:
	AutoCourse_t course_regulator;
	AutoAngularVelocity_t velocity_regulator;

	bool _use_auto_cource;
public:
	AutoRotation_t(MotorsForceDistributor_t* force_distrib, SensorManager_t* sensor_manager, 
		PidCoefficient_t y_course_pid, PidCoefficient_t z_course_pid, 
			PidCoefficient_t y_autovelocity_pid, PidCoefficient_t z_autovelocity_pid) :
	course_regulator(force_distrib, sensor_manager, y_course_pid, z_course_pid),
	velocity_regulator(force_distrib, sensor_manager, y_autovelocity_pid, z_autovelocity_pid) {}

	void SetRotate(float y, float z) {
		course_regulator.SetRotate(y, z);
		_use_auto_cource = true;
	}

	void SetAngularVelocity(float y, float z) {
		velocity_regulator.SetAngularVelocity(y, z);
		_use_auto_cource = false;
	}

	void Update() {
		if (_use_auto_cource) {
			course_regulator.Update();
		} else {
			velocity_regulator.Update();
		}
	}
};

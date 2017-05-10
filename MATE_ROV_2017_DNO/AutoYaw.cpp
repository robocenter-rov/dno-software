#include "AutoYaw.h"

void AutoYaw_t::Update(MotorsForceDistributor_t* motors_force_distributor) {
	float q1, q2, q3, q4;
	_sensor_manager->GetRotation(q1, q2, q3, q4);

	_input = atan2(2 * q2 * q3 - 2 * q1 * q4, 2 * q1 * q1 + 2 * q2 * q2 - 1);

	_pid.ComputeYaw();

	motors_force_distributor->AddLocalRotateForce(0, _output);
}

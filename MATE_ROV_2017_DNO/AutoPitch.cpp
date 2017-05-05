#include "AutoPitch.h"

void AutoPitch_t::Update(MotorsForceDistributor_t* motors_force_distributor) {
	float q0, q1, q2, q3;
	_sensor_manager->GetRotation(q0, q1, q2, q3);

	_input = atan2(2 * q1 * q2 - 2 * q0 * q3, 2 * q0 * q0 + 2 * q1 * q1 - 1);;
	_pid.Compute();

	motors_force_distributor->AddLocalRotateForce(0, _output);
}

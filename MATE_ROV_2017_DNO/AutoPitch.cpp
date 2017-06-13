#include "AutoPitch.h"

void AutoPitch_t::Update(MotorsForceDistributor_t* motors_force_distributor) {
	_input = _sensor_manager->Rotation()->GetPitch();

	_pid.Compute();

	motors_force_distributor->AddLocalRotateForce(_output, 0, 0);
}

#include "AutoYaw.h"

void AutoYaw_t::Update(MotorsForceDistributor_t* motors_force_distributor) {
	_input = _sensor_manager->Rotation()->GetYaw();

	_pid.ComputeYaw();

	motors_force_distributor->AddLocalRotateForce(0, 0, _output);
}

#include "AutoRoll.h"

void AutoRoll_t::Update(MotorsForceDistributor_t* motors_force_distributor) {
	_input = _sensor_manager->Rotation()->GetRoll();

	_pid.Compute();

	motors_force_distributor->AddLocalRotateForce(0, _output, 0);
}
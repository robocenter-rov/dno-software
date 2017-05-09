#include "ManipulatorPeriphery.h"

ManipulatorPeriphery_t::ManipulatorPeriphery_t(PCA96685Motor_t* hand, PCA96685Motor_t* arm)
{
	_hand = hand;
	_arm = arm;
}

void ManipulatorPeriphery_t::SetHandThrust(float thrust) const
{
	_hand->SetThrust(thrust);
}

void ManipulatorPeriphery_t::SetArmThrust(float thrust) const
{
	_arm->SetThrust(thrust);
}

void ManipulatorPeriphery_t::Stop() const {
	_arm->SetThrust(0);
	_hand->SetThrust(0);
}

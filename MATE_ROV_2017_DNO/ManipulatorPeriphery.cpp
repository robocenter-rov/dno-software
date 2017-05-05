#include "ManipulatorPeriphery.h"

/*ManipulatorPeriphery_t::ManipulatorPeriphery_t(PCA9685ServoMotor_t* hand, PCA9685ServoMotor_t* arm) {
	_hand = hand;
	_arm = arm;
}

void ManipulatorPeriphery_t::SetHandAngle(float angle_radians) const {
	_hand->SetAngle(angle_radians);
}

void ManipulatorPeriphery_t::SetArmAngle(float ange_radians) const {
	_arm->SetAngle(ange_radians);
}*/


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

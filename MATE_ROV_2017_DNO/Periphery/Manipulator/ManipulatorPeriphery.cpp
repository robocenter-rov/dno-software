#include "ManipulatorPeriphery.h"

ManipulatorPeriphery_t::ManipulatorPeriphery_t(PCA9685ServoMotor_t* hand, PCA9685ServoMotor_t* arm) {
	_hand = hand;
	_arm = arm;
}

void ManipulatorPeriphery_t::SetHandAngle(float angle_radians) const {
	_hand->SetAngle(angle_radians);
}

void ManipulatorPeriphery_t::SetArmAngle(float ange_radians) const {
	_arm->SetAngle(ange_radians);
}

#pragma once
#include "PCA9685ServoMotor.h"

class ManipulatorPeriphery_t {
private:
	PCA9685ServoMotor_t* _hand;
	PCA9685ServoMotor_t* _arm;
public:
	ManipulatorPeriphery_t(PCA9685ServoMotor_t* hand, PCA9685ServoMotor_t* arm);

	void SetHandAngle(float angle_radians) const;
	void SetArmAngle(float ange_radians) const;
};

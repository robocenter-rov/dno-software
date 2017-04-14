#pragma once
//#include "PCA9685ServoMotor.h"
#include "PCA96685Motor.h"

class ManipulatorPeriphery_t {
private:
	PCA96685Motor_t* _hand;
	PCA96685Motor_t* _arm;
	/*PCA9685ServoMotor_t* _hand;
	PCA9685ServoMotor_t* _arm;*/
public:
	ManipulatorPeriphery_t(PCA96685Motor_t* hand, PCA96685Motor_t* arm);
	/*ManipulatorPeriphery_t(PCA9685ServoMotor_t* hand, PCA9685ServoMotor_t* arm);

	void SetHandAngle(float angle_radians) const;
	void SetArmAngle(float ange_radians) const;*/

	void SetHandThrust(unsigned int thrust) const;
	void SetArmThrust(unsigned int thrust) const;
};

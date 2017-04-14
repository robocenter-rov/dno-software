#pragma once
#include "PCA9685ServoMotor.h"

class CameraPeriphery_t {
private:
	PCA9685ServoMotor_t* _servo1;
	PCA9685ServoMotor_t* _servo2;
public:
	CameraPeriphery_t(PCA9685ServoMotor_t* servo1, PCA9685ServoMotor_t* servo2);
	void SetAngleServo1(float angle_radians);
	void SetAngleServo2(float angle_radians);
	void SetAngle(PCA9685ServoMotor_t* servo, float angle_radians) const;
};

#pragma once
#include <Adafruit_PWMServoDriver.h>

class PCA9685ServoMotor_t {
private:
	Adafruit_PWMServoDriver* _pwm;
	unsigned int _pwmNum;
public:
	PCA9685ServoMotor_t(Adafruit_PWMServoDriver* pwm, unsigned int pwnNum);

	void SetAngle(float angle_radians) const;
};
#pragma once
#include <Adafruit_PWMServoDriver.h>

class PCA9685ServoMotor_t {
private:
	Adafruit_PWMServoDriver* _pwm;
	unsigned int _pwmNum;
	unsigned int _maxPwmVal;
public:
	PCA9685ServoMotor_t(Adafruit_PWMServoDriver* pwm, unsigned int pwnNum, unsigned int maxPwmVal = 4095);

	void SetAngle(float angle_radians) const;
};

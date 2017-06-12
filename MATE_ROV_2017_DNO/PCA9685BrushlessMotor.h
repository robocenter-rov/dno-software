#pragma once

#include "Motor.h"
#include "Libs/Adafruit_PWMServoDriver/Adafruit_PWMServoDriver.h"

class PCA9685BrushlessMotor : public Motor_t {
private:
	Adafruit_PWMServoDriver *_pwm;
	int _pwmNum;
	unsigned int _maxPwmVal;
	unsigned int _minPwmVal;

	void _SetThrust() override;
public:
	PCA9685BrushlessMotor(Adafruit_PWMServoDriver *pwm, unsigned int pwmNum, 
		unsigned minPwmVal = 0, unsigned maxPwmVal = 4095);
	~PCA9685BrushlessMotor();
};


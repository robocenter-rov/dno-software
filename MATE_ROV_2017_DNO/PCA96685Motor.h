#pragma once

#include "Motor.h"
#include "Libs/Adafruit_PWMServoDriver/Adafruit_PWMServoDriver.h"

class PCA96685Motor_t : public Motor_t {
private:
	Adafruit_PWMServoDriver *_pwm;
	unsigned int _INAPwmNum;
	unsigned int _INBPwnNum;
	int _pwmNum;
	unsigned int _maxPwmVal = 4095;

	void _SetThrust() override;
public:
	PCA96685Motor_t(Adafruit_PWMServoDriver *pwm, unsigned int INAPwmNum, unsigned int INBPwmNum, unsigned int pwmNum, unsigned int maxPwmVal = 4095);
	~PCA96685Motor_t();
};

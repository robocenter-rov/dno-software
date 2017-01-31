#pragma once

#include "Motor.h"
#include <Adafruit_PWMServoDriver.h>

class PCA96685Motor_t : public Motor_t {
private:
	Adafruit_PWMServoDriver *_pwm;
	unsigned int _INAPwmNum;
	unsigned int _INBPwnNum;
	int _pwmNum;
public:
	PCA96685Motor_t(Adafruit_PWMServoDriver *pwm, unsigned int INAPwmNum, unsigned int INBPwmNum, unsigned int pwmNum);
	~PCA96685Motor_t();
	void SetThrust(float thrust) override;
};

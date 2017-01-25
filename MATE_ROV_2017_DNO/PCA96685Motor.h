#pragma once

#include "Motor.h"
#include <Adafruit_PWMServoDriver.h>

class PCA96685Motor_t : public Motor_t {
private:
	Adafruit_PWMServoDriver *_pwm;
	unsigned int _pinA;
	unsigned int _pinB;
	int _pwmNum;
public:
	PCA96685Motor_t(Adafruit_PWMServoDriver *pwm, unsigned int PINa, unsigned int PINb, unsigned int pwmNum);
	~PCA96685Motor_t();
	void SetThrust(float thrust) override;
};

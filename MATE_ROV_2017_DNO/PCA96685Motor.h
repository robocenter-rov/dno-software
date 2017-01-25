#pragma once

#include "Arduino.h"
#include "Motor.h"
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>


class PCA96685Motor_t : public Motor_t
{
private:
	Adafruit_PWMServoDriver *_pwm;
	unsigned int _PINA;
	unsigned int _PINB;
	int _pwmNum;
public:
	PCA96685Motor_t(Adafruit_PWMServoDriver *pwm, unsigned int PINa, unsigned int PINb, int pwmNum);
	~PCA96685Motor_t();
	void SetThrust(float thrust) override;
};

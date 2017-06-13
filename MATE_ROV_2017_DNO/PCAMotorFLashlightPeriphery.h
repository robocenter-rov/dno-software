#pragma once

#include "Libs/Adafruit_PWMServoDriver/Adafruit_PWMServoDriver.h"
#include "FlashlightPeriphery.h"

class PCAMotorFlashlightPeriphery_t : public FlashlightPeriphery_t {
private:
	Adafruit_PWMServoDriver* _pwm;
	int _pwmNum;
	int _pwmANum;
	int _pwmBNum;
	int _onVal;

	void _TurnOn() override {
		_pwm->setPWM(_pwmNum, 0, _onVal);
		_pwm->setPWM(_pwmANum, 0, 4095);
		_pwm->setPWM(_pwmBNum, 0, 0);
	}

	void _TurnOff() override {
		_pwm->setPWM(_pwmNum, 0, 0);
		_pwm->setPWM(_pwmANum, 0, 0);
		_pwm->setPWM(_pwmBNum, 0, 0);
	}
public:
	PCAMotorFlashlightPeriphery_t(Adafruit_PWMServoDriver* pwm, int pwmNum, int pwmANum, int pwmBNum, int onVal) 
		: _pwm(pwm), _pwmNum(pwmNum), _pwmANum(pwmANum), _pwmBNum(pwmBNum), _onVal(onVal) {}
};
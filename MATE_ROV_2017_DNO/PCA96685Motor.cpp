#include "Debug.h"
#include "PCA96685Motor.h"

PCA96685Motor_t::PCA96685Motor_t(Adafruit_PWMServoDriver *pwm, unsigned int INAPwmNum, unsigned int INBPwmNum, unsigned int pwmNum, unsigned int maxPwmVal = 4095)
{
	_pwm = pwm;
	_INAPwmNum = INAPwmNum;
	_INBPwnNum = INBPwmNum;
	_pwmNum = pwmNum;
	_maxPwmVal = maxPwmVal;
}

PCA96685Motor_t::~PCA96685Motor_t() {
}

void PCA96685Motor_t::_SetThrust() {
	LOG("Setting motor ");
	LOG(_pwmNum);
	LOG(": ");
	LOGLN(_maxPwmVal * min(abs(_thrust * _mul), 1));

	_pwm->setPWM(_pwmNum, 0, _maxPwmVal * min(abs(_thrust * _mul), 1));

	if (_thrust > 0) {
		_pwm->setPWM(_INAPwmNum, 0, 4095);
		_pwm->setPWM(_INBPwnNum, 0, 0);
	} else {
		_pwm->setPWM(_INAPwmNum, 0, 0);
		_pwm->setPWM(_INBPwnNum, 0, 4095);
	}
}

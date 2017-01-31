#include "PCA96685Motor.h"

PCA96685Motor_t::PCA96685Motor_t(Adafruit_PWMServoDriver *pwm, unsigned int INAPwmNum, unsigned int INBPwmNum, unsigned int pwmNum)
{
	_pwm = pwm;
	_INAPwmNum = INAPwmNum;
	_INBPwnNum = INBPwmNum;
	_pwmNum = pwmNum;
}

PCA96685Motor_t::~PCA96685Motor_t() {
}

void PCA96685Motor_t::SetThrust(float thrust) 
{
#ifdef _DEBUG
	Serial.print("Setting motor ");
	Serial.print(_pwmNum);
	Serial.print(": ");
	Serial.println(4095 * min(abs(thrust), 1));
#endif

	_pwm->setPWM(_pwmNum, 0, 4095 * min(abs(thrust),1));

	if (thrust > 0) {
		_pwm->setPWM(_INAPwmNum, 0, 4095);
		_pwm->setPWM(_INBPwnNum, 0, 0);
	} else {
		_pwm->setPWM(_INAPwmNum, 0, 0);
		_pwm->setPWM(_INBPwnNum, 0, 4095);
	}
}

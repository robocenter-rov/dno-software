#include "PCA96685Motor.h"



PCA96685Motor_t::PCA96685Motor_t(Adafruit_PWMServoDriver *pwm, unsigned int PINa, unsigned int PINb, int pwmNum)
{
	_pwm = pwm;
	_PINA = PINa;
	_PINB = PINb;
	_pwmNum = pwmNum;
}

PCA96685Motor_t::~PCA96685Motor_t()
{
}

void PCA96685Motor_t::SetThrust(float thrust) 
{
	_pwm->setPWM(_pwmNum, 0, 4096 * min(abs(thrust),1));

	if (thrust > 0) {
		digitalWrite(_PINA, 1);
		digitalWrite(_PINB, 0);
	}
	else {
		digitalWrite(_PINA, 0);
		digitalWrite(_PINB, 1);
	}
	
}

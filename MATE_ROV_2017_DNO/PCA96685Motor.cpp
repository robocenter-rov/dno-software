#include "PCA96685Motor.h"

PCA96685Motor_t::PCA96685Motor_t(Adafruit_PWMServoDriver *pwm, unsigned int pin_a, unsigned int pin_b, unsigned int pwmNum)
{
	_pwm = pwm;
	_pinA = pin_a;
	_pinB = pin_b;
	_pwmNum = pwmNum;

	pinMode(_pinA, OUTPUT);
	pinMode(_pinB, OUTPUT);
}

PCA96685Motor_t::~PCA96685Motor_t()
{
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
		digitalWrite(_pinA, 1);
		digitalWrite(_pinB, 0);
	}
	else {
		digitalWrite(_pinA, 0);
		digitalWrite(_pinB, 1);
	}
	
}

#include "PCA9685BrushlessMotor.h"


void PCA9685BrushlessMotor::_SetThrust()
{
	float thrust = _thrust * _mul;
	float k = (_maxPwmVal - _minPwmVal) / 2;
	_pwm->setPWM(_pwmNum, 0, thrust * k + (_maxPwmVal - k));
}

PCA9685BrushlessMotor::PCA9685BrushlessMotor(Adafruit_PWMServoDriver* pwm, unsigned pwmNum, 
	unsigned maxPwmVal = 4095, unsigned minPwmVal = 0)
{
	_pwm = pwm;
	_pwmNum = pwmNum;
	_maxPwmVal = maxPwmVal;
	_minPwmVal = minPwmVal;
}


PCA9685BrushlessMotor::~PCA9685BrushlessMotor()
{
}

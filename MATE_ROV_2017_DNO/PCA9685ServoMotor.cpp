#include "Debug.h"
#include "PCA9685ServoMotor.h"
#include "MathConstants.h"

PCA9685ServoMotor_t::PCA9685ServoMotor_t(Adafruit_PWMServoDriver* pwm, unsigned int pwmNum, unsigned int maxPwmVal, 
	float offset, bool direction, float angle) {
	_pwm = pwm;
	_pwmNum = pwmNum;
	_maxPwmVal = maxPwmVal;
	_minVal = 150;
	_maxVal = 600;
	_angle = angle;
}

void PCA9685ServoMotor_t::SetAngle(float angle_radians) {
	_angle = angle_radians;

	int pwmVal = (_angle + (PI / 2)) * ((_maxVal - _minVal) / PI) + _minVal;

	LOG("Setting pwmNum ");
	LOG(_pwmNum);
	LOG(" to ");
	LOGLN(pwmVal);

	_pwm->setPWM(_pwmNum, 0, pwmVal);
}

void PCA9685ServoMotor_t::SetMinVal(float minVal) {
	_minVal = minVal;
}

void PCA9685ServoMotor_t::SetMaxVal(float maxVal) {
	_maxVal = maxVal;
}

float PCA9685ServoMotor_t::GetAngle() const
{
	return _angle;
}

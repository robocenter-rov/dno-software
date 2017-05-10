#include "Debug.h"
#include "PCA9685ServoMotor.h"
#include "MathConstants.h"

PCA9685ServoMotor_t::PCA9685ServoMotor_t(Adafruit_PWMServoDriver* pwm, unsigned int pwmNum, unsigned int maxPwmVal, 
	float offset, bool direction, float angle) {
	_pwm = pwm;
	_pwmNum = pwmNum;
	_maxPwmVal = maxPwmVal;
	_direction = direction;
	_offset = offset;
	_angle = angle;
}

void PCA9685ServoMotor_t::SetAngle(float angle_radians) {
	int pwmVal;
	_angle = angle_radians;
	if (_direction)
	{
		pwmVal = (constrain(angle_radians, -PI, PI) + PI) / PI2 * 500;
	} else
	{
		pwmVal = (constrain((angle_radians + _offset) * _direction? 1 : (-1) , -PI, PI) + PI) / PI2 * 500;
	}

	LOG("Setting pwmNum ");
	LOG(_pwmNum);
	LOG(" to ");
	LOGLN(pwmVal);

	_pwm->setPWM(_pwmNum, 0, pwmVal);
}

void PCA9685ServoMotor_t::SetOffset(float offset)
{
	_offset = constrain(offset, -PI, PI);
}

void PCA9685ServoMotor_t::SetDirection(bool direction)
{
	_direction = direction;
}

float PCA9685ServoMotor_t::GetAngle() const
{
	return _angle;
}

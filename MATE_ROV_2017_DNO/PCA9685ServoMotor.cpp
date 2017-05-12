#include "Debug.h"
#include "PCA9685ServoMotor.h"
#include "MathConstants.h"

PCA9685ServoMotor_t::PCA9685ServoMotor_t(SensorRotation_t* sensor_rotation, Adafruit_PWMServoDriver* pwm, unsigned int pwmNum, unsigned int maxPwmVal,
	float offset, bool direction, float angle) {

	_pwm = pwm;
	_pwmNum = pwmNum;
	_maxPwmVal = maxPwmVal;
	_minVal = 150;
	_maxVal = 600;
	_angle = angle;
	_sensor_rotation = sensor_rotation;
	_local = true;
}

void PCA9685ServoMotor_t::_SetAngle(float angle_radians) {
	_angle = angle_radians;

	int pwmVal = (angle_radians + (PI / 2)) * ((_maxVal - _minVal) / PI) + _minVal;

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

void PCA9685ServoMotor_t::SetGlobalAngle(float globalAngle)
{
	_angle = globalAngle;
	_local = false;
}

void PCA9685ServoMotor_t::SetLocalAngle(float localAngle)
{
	_angle = localAngle;
	_local = true;
}

void PCA9685ServoMotor_t::Update()
{
	if (_local) return;

	float pitch = _sensor_rotation->GetPithch();
	_SetAngle(_angle - pitch);
}

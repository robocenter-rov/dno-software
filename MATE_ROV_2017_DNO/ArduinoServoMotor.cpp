#include "ArduinoServoMotor.h"
#include "MathConstants.h"

ArduinoServoMotor_t::~ArduinoServoMotor_t()
{
}

ArduinoServoMotor_t::ArduinoServoMotor_t(int pin, float minVal, float maxVal) {
	_minVal = minVal;
	_maxVal = maxVal;
	_myservo.attach(pin);
}

void ArduinoServoMotor_t::SetAngle(float angle_radians)
{
	float degreesAngle;
	_angle = angle_radians;
	degreesAngle = (_angle + (PI / 2)) * ((_maxVal - _minVal) / PI) + _minVal;
	_myservo.write(degreesAngle);
}

void ArduinoServoMotor_t::SetMinVal(float minVal)
{
	_minVal = minVal;
}

void ArduinoServoMotor_t::SetMaxVal(float maxVal)
{
	_maxVal = maxVal;
}

float ArduinoServoMotor_t::GetAngle() const {
	return _angle;
}



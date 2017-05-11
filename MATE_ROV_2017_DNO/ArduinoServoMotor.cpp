#include "ArduinoServoMotor.h"
#include "MathConstants.h"

ArduinoServoMotor_t::~ArduinoServoMotor_t()
{
}

ArduinoServoMotor_t::ArduinoServoMotor_t(int pin, unsigned pwnNum, float offset, bool direction, float angle)
{
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



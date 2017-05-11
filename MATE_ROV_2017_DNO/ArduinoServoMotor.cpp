#include "ArduinoServoMotor.h"
#include "MathConstants.h"

ArduinoServoMotor_t::~ArduinoServoMotor_t()
{
}

ArduinoServoMotor_t::ArduinoServoMotor_t(SensorRotation_t* sensor_rotation, int pin, float minVal, float maxVal) {
	_minVal = minVal;
	_maxVal = maxVal;
	_myservo.attach(pin);
	_sensor_rotation = sensor_rotation;
	_local = true;
}

void ArduinoServoMotor_t::_SetAngle(float angle_radians)
{
	float degreesAngle;
	degreesAngle = (angle_radians + (PI / 2)) * ((_maxVal - _minVal) / PI) + _minVal;
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

void ArduinoServoMotor_t::SetGlobalAngle(float globalAngle)
{
	_angle = globalAngle;
	_local = false;
}

void ArduinoServoMotor_t::SetLocalAngle(float localAngle)
{
	_angle = localAngle;
	_local - true;
}

void ArduinoServoMotor_t::Update()
{
	if (_local) return;

	float pitch = _sensor_rotation->GetPithch();
	_SetAngle(_angle - pitch);
}



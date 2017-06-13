#include "ArduinoServoMotor.h"

ArduinoServoMotor_t::ArduinoServoMotor_t(SensorRotation_t* sensor_rotation, int pin, float minVal, float maxVal) : ServoPeriphery_t(sensor_rotation, minVal, maxVal) {
	_myservo.attach(pin);
}

void ArduinoServoMotor_t::SetAngle(float angle)
{
	_myservo.write(angle);
}

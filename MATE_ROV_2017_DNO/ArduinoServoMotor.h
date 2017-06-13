#pragma once
#include <Servo.h>
#include "SensorRotation.h"
#include "SensorManager.h"
#include "ServoPeriphery.h"

class ArduinoServoMotor_t : public ServoPeriphery_t
{
private:
	Servo _myservo;
	void SetAngle(float angle_radians) override;
public:
	ArduinoServoMotor_t(SensorRotation_t* sensor_rotation, int pin, float minVal = 0, float maxVal = 180);
	~ArduinoServoMotor_t() = default;
};


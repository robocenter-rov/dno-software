#pragma once
#include <Servo.h>
#include "SensorRotation.h"
#include "SensorManager.h"

class ArduinoServoMotor_t
{
private:
	SensorRotation_t* _sensor_rotation;
	Servo _myservo;
	float _minVal;
	float _maxVal;
	float _angle;
	bool _local;

	void _SetAngle(float angle_radians);

public:
	ArduinoServoMotor_t(SensorRotation_t* sensor_rotation, int pin, float minVal = 0, float maxVal = 180);
	void SetMinVal(float minVal);
	void SetMaxVal(float maxVal);
	float GetAngle() const;
	void SetGlobalAngle(float globalAngle);
	void SetLocalAngle(float localAngle);
	void Update();

	~ArduinoServoMotor_t();
};


#pragma once
#include <Servo.h>

class ArduinoServoMotor_t
{
private:
	Servo _myservo;
	float _minVal;
	float _maxVal;
	float _angle;
public:
	ArduinoServoMotor_t(int pin, unsigned int pwnNum, float offset = 0, bool direction = false, float angle = 0);
	void SetAngle(float angle_radians);
	void SetMinVal(float minVal);
	void SetMaxVal(float maxVal);
	float GetAngle() const;
	~ArduinoServoMotor_t();
};


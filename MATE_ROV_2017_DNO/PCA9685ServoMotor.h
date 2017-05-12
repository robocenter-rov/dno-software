#pragma once
#include "Libs/Adafruit_PWMServoDriver/Adafruit_PWMServoDriver.h"
#include "SensorRotation.h"

class PCA9685ServoMotor_t {
private:
	SensorRotation_t* _sensor_rotation;
	Adafruit_PWMServoDriver* _pwm;
	unsigned int _pwmNum;
	unsigned int _maxPwmVal;
	float _minVal;
	float _maxVal;
	float _angle;
	bool _local;

	void _SetAngle(float angle_radians);
public:
	PCA9685ServoMotor_t(SensorRotation_t* sensor_rotation, Adafruit_PWMServoDriver* pwm, unsigned int pwnNum, unsigned int maxPwmVal = 4095,
		float offset = 0, bool direction = false, float angle = 0);
	void SetMinVal(float minVal);
	void SetMaxVal(float maxVal);
	float GetAngle() const;
	void SetGlobalAngle(float globalAngle);
	void SetLocalAngle(float localAngle);
	void Update();
};

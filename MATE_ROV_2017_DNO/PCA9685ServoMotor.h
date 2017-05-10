#pragma once
#include "Libs/Adafruit_PWMServoDriver/Adafruit_PWMServoDriver.h"

class PCA9685ServoMotor_t {
private:
	Adafruit_PWMServoDriver* _pwm;
	unsigned int _pwmNum;
	unsigned int _maxPwmVal;
	float _offset;
	bool _direction;
	float _angle;
public:
	PCA9685ServoMotor_t(Adafruit_PWMServoDriver* pwm, unsigned int pwnNum, unsigned int maxPwmVal = 4095, 
		float offset = 0, bool direction = false, float angle = 0);
	void SetAngle(float angle_radians);
	void SetOffset(float offset);
	void SetDirection(bool direction);
	float GetAngle() const;
};

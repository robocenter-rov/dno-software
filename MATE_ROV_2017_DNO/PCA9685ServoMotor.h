#pragma once
#include "Libs/Adafruit_PWMServoDriver/Adafruit_PWMServoDriver.h"
#include "SensorRotation.h"
#include "ServoPeriphery.h"

class PCA9685ServoMotor_t :public ServoPeriphery_t {
private:
	Adafruit_PWMServoDriver* _pwm;
	unsigned int _pwmNum;
	void SetAngle(float val) override;
public:
	PCA9685ServoMotor_t(SensorRotation_t* sensor_rotation, Adafruit_PWMServoDriver* pwm, unsigned int pwnNum, int minVal = 150, int maxVal = 600);
	~PCA9685ServoMotor_t() = default;
};

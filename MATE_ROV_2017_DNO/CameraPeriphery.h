#pragma once

class CameraPeriphery_t {
private:
	PCA9685ServoMotor_t* _servo1;
	PCA9685ServoMotor_t* _servo2;
public:
	CameraPeriphery_t(PCA9685ServoMotor_t* servo1, PCA9685ServoMotor_t* servo2);
};
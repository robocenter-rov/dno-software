#include "Debug.h"
#include "PCA9685ServoMotor.h"
#include "MathConstants.h"

PCA9685ServoMotor_t::PCA9685ServoMotor_t(SensorRotation_t* sensor_rotation, Adafruit_PWMServoDriver* pwm, unsigned int pwmNum, int minVal = 150, int maxVal = 600) 
 : ServoPeriphery_t(sensor_rotation, minVal, maxVal) {
	_pwm = pwm;
	_pwmNum = pwmNum;
}

void PCA9685ServoMotor_t::SetAngle(float val) {
	LOG("Setting pwmNum ");
	LOG(_pwmNum);
	LOG(" to ");
	LOGLN(val);

	_pwm->setPWM(_pwmNum, 0, val);
}

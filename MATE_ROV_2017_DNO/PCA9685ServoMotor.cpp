#include "PCA9685ServoMotor.h"
#include "MathConstants.h"

PCA9685ServoMotor_t::PCA9685ServoMotor_t(Adafruit_PWMServoDriver* pwm, unsigned int pwmNum) {
	_pwm = pwm;
	_pwmNum = pwmNum;
}

void PCA9685ServoMotor_t::SetAngle(float angle_radians) const {
	int pwmVal = constrain(angle_radians, 0, PI2) / PI2 * 500;

#ifdef _DEBUG
	Serial.print("Setting pwmNum ");
	Serial.print(_pwmNum);
	Serial.print(" to ");
	Serial.println(pwmVal);
#endif

	_pwm->setPWM(_pwmNum, 0, pwmVal);
}

#include "Debug.h"
#include "PCA9685ServoMotor.h"
#include "MathConstants.h"

PCA9685ServoMotor_t::PCA9685ServoMotor_t(Adafruit_PWMServoDriver* pwm, unsigned int pwmNum, unsigned int maxPwmVal) {
	_pwm = pwm;
	_pwmNum = pwmNum;
	_maxPwmVal = maxPwmVal;
}

void PCA9685ServoMotor_t::SetAngle(float angle_radians) const {
	int pwmVal = constrain(angle_radians, 0, PI2) / PI2 * 500;

	LOG("Setting pwmNum ");
	LOG(_pwmNum);
	LOG(" to ");
	LOGLN(pwmVal);

	_pwm->setPWM(_pwmNum, 0, pwmVal);
}

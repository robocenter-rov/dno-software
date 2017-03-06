#pragma once
#include "../../Movement/Motor.h"
#include "../../libs/DualVNH5019MotorShield/DualVNH5019MotorShield.h"

class PololuMotor_t : public Motor_t {
private:
	DualVNH5019MotorShield _md;
public:
	PololuMotor_t(unsigned char INA, unsigned char INB);

	void SetThrust(float thrust) override;
};
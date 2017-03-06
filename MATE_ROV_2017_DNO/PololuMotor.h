#pragma once
#include "Motor.h"
#include <DualVNH5019MotorShield.h>

class PololuMotor_t : public Motor_t {
private:
	DualVNH5019MotorShield _md;
public:
	PololuMotor_t(unsigned char INA, unsigned char INB);

	void SetThrust(float thrust) override;
};
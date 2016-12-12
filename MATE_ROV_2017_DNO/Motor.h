#pragma once

#include <DualVNH5019MotorShield.h>

class Motor_t {
public:
	Motor_t();
	
	virtual void SetThrust(float thrust);

	virtual ~Motor_t();
};

class PololuMotor_t : public Motor_t {
private:
	DualVNH5019MotorShield _md;
public:
	PololuMotor_t(unsigned char INA, unsigned char INB);

	void SetThrust(float thrust) override;
};

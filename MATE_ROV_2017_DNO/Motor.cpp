#include "Motor.h"

Motor_t::Motor_t() {
	_thrust = 0;
	_mul = 1;
}

void Motor_t::SetThrust(float thrust) {}

float Motor_t::GetThrust() const {
	return _thrust;
}

void Motor_t::SetMultiplier(float mul) {
	_mul = mul;
}

Motor_t::~Motor_t() {}

#include "DebugMotorsMain.h"
#include "Arduino.h"

DebugMotorsMain_t::DebugMotorsMain_t(Motor_t* motor) {
	_motor = motor;
}

void DebugMotorsMain_t::Loop() {
	_motor->SetThrust(1);
	delay(2000);
	_motor->SetThrust(-1);
	delay(2000);
}

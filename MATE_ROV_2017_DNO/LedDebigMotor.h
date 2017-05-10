#pragma once

#include "Arduino.h"
#include "Motor.h"

class LedDebugMotor_t : public Motor_t {
private:
	int _pin;
	
	void _SetThrust() override {
		analogWrite(_pin, round(constrain(abs(_thrust * _mul), 0, 1) * 255));
	}
public:
	LedDebugMotor_t(int pin) {
		_pin = pin;
		pinMode(pin, OUTPUT);
	}
};

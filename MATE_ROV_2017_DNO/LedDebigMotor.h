#pragma once

#include "Arduino.h"
#include "Motor.h"

class LedDebugMotor_t : public Motor_t {
private:
	int _pin;
public:
	LedDebugMotor_t(int pin) {
		_pin = pin;
		pinMode(pin, OUTPUT);
	}

	void SetThrust(float thrust) override {
		analogWrite(_pin, round(constrain(abs(thrust * _mul), 0, 1) * 255));
	}
};

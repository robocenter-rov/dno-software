#pragma once

#include "Arduino.h"

class FlashlightPeriphery_t {
private:
	int _pin;
	bool _state;
public:
	FlashlightPeriphery_t(int pin) {
		_pin = pin;
		_state = false;
		pinMode(_pin, OUTPUT);
	}

	void TurnOn() {
#ifdef _DEBUG
		Serial.println("Turning ON flashlight");
#endif
		digitalWrite(_pin, HIGH);
		_state = true;
	}

	void TurnOff() {
#ifdef _DEBUG
		Serial.println("Turning OFF flashlight");
#endif
		digitalWrite(_pin, LOW);
		_state = false;
	}

	void SetState(bool state) {
		if (state) {
			TurnOn();
		} else {
			TurnOff();
		}
	}

	bool GetState() const {
		return _state;
	}
};
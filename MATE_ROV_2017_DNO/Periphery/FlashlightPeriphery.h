#pragma once

#include "Arduino.h"
#include "../Utils/Debug.h"

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

		LOGLN("Turning ON flashlight");

		digitalWrite(_pin, HIGH);
		_state = true;
	}

	void TurnOff() {

		LOGLN("Turning OFF flashlight");

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

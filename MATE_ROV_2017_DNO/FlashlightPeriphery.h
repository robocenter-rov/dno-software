#pragma once

#include "Arduino.h"
#include "Debug.h"

class FlashlightPeriphery_t {
private:
	bool _state;

	virtual void _TurnOn() = 0;
	virtual void _TurnOff() = 0;
public:
	virtual ~FlashlightPeriphery_t() = default;

	FlashlightPeriphery_t() {
		_state = false;
	}

	void TurnOn() {
		LOGLN("Turning ON flashlight");
		_TurnOn();
		_state = true;
	}

	void TurnOff() {
		LOGLN("Turning OFF flashlight");
		_TurnOff();
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

#pragma once

class FlashlightPeriphery_t {
private:
	int _pin;
	bool _state;
public:
	FlashlightPeriphery_t(int pin) {
		_pin = pin;
		pinMode(_pin, OUTPUT);
	}

	void TurnOn() {
		digitalWrite(_pin, HIGH);
		_state = true;
	}

	void TurnOff() {
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
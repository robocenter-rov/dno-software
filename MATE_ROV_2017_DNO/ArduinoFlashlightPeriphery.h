#pragma once

class ArduinoFlashlightPeriphery_t : public FlashlightPeriphery_t {
private:
	int _pin;

	void _TurnOn() override {
		digitalWrite(_pin, HIGH);
	}

	void _TurnOff() override {
		digitalWrite(_pin, LOW);
	}
public:
	ArduinoFlashlightPeriphery_t(int pin) : _pin(pin) {
		pinMode(_pin, OUTPUT);
	}
};

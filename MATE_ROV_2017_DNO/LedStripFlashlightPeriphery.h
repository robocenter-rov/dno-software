#pragma once
#include "Libs/FastLED/FastLED.h"

class LedStripFlashlightPeriphery_t : public FlashlightPeriphery_t {
private:
	CRGB* _leds;
	int _num_leds;
	LedStripFlashlightPeriphery_t() : _leds(nullptr), _num_leds(0) {}
public:
	void SetColor(CRGB color) {
		for (int i = 0; i < _num_leds; i++) {
			_leds[i] = color;
		}
		FastLED.show();
	}

	template<int dataPin>
	static LedStripFlashlightPeriphery_t* create(int num_leds) {
		auto t = new LedStripFlashlightPeriphery_t;
		t->_num_leds = num_leds;
		t->_leds = new CRGB[num_leds];
		FastLED.addLeds<WS2811, dataPin, RGB>(t->_leds, num_leds);
		return t;
	}

	~LedStripFlashlightPeriphery_t() {
		delete[] _leds;
	}
private:
	void _TurnOn() override {
		SetColor(CRGB::LightYellow);
	}
	
	void _TurnOff() override {
		SetColor(CRGB::Black);
	}
};
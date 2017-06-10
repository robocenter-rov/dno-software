#pragma once
#include "Libs/FastLED/FastLED.h"

class LEDStripLightPeriphery_t
{
private:
	int _dataPin;
	int _ledsNum;
	CRGB* _ledStrip;


public:
	LEDStripLightPeriphery_t(int dataPin, int ledsNum);
	~LEDStripLightPeriphery_t();
	void SetColor(float thrust_y, float thrust_x, float thrust_z);
};


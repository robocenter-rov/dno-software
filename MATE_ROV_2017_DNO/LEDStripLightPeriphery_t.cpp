#include "LEDStripLightPeriphery_t.h"
//#include "Libs/FastLED/FastLED.h"


LEDStripLightPeriphery_t::LEDStripLightPeriphery_t(int dataPin, int ledsNum)
{
	_dataPin = dataPin;
	_ledsNum = ledsNum;
	_ledStrip = new CRGB[ledsNum];


	FastLED.addLeds<WS2811, dataPin, RGB>(_ledStrip, _ledsNum);
}

LEDStripLightPeriphery_t::~LEDStripLightPeriphery_t()
{
}

void LEDStripLightPeriphery_t::SetColor(float thrust_y, float thrust_x, float thrust_z)
{
	int y = thrust_y * 127.5 + 127.5;
	int x = thrust_x  * 127.5 + 127.5;
	int z = thrust_z  * 127.5 + 127.5;
	for (int i = 0; i< _ledsNum; i++)
	{
		_ledStrip[i] = CRGB(y, x, z);
	}
	FastLED.show();
}

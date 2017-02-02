#include "PololuMotor.h"

PololuMotor_t::PololuMotor_t(unsigned char INA, unsigned char INB) : _md(INA, INB, 0, 0, 0, 0, 0, 0) {
#ifdef _DEBUG
	if (_md.getM1Fault()) {
		Serial.println("Failed initialize motor");
	}
	else {
		Serial.println("Successfully initialize motor");
	}
	delay(200);
#endif
}

void PololuMotor_t::SetThrust(float thrust) {

	LOG("Set thrust: ");
	LOGLN(thrust);

	_md.setM1Speed(400 * thrust);
}



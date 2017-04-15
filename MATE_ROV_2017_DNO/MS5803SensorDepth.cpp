#include "MS5803SensorDepth.h"

MS5803SensorDepth_t::MS5803SensorDepth_t(ms5803_addr adress, precision precision) : _sensor(adress) {
	_precision = precision;

	_sensor.reset();
	_sensor.begin();
}

float MS5803SensorDepth_t::GetRawData() {
	return _sensor.getPressure(_precision);
}

float MS5803SensorDepth_t::GetDepth() {
	return GetRawData();
}

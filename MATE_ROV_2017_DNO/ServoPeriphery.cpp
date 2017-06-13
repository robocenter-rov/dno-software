#include "ServoPeriphery.h"

ServoPeriphery_t::ServoPeriphery_t(SensorRotation_t* sensor_rotation, float minVal, float maxVal) {
	_minVal = minVal;
	_maxVal = maxVal;
	_angle = 0;
	_local = true;
	_sensor_rotation = sensor_rotation;
}

void ServoPeriphery_t::SetMinVal(float minVal) {
	_minVal = minVal;
}

void ServoPeriphery_t::SetMaxVal(float maxVal) {
	_maxVal = maxVal;
}

float ServoPeriphery_t::GetAngle() const {
	return _angle;
}

void ServoPeriphery_t::SetGlobalAngle(float globalAngle) {
	_angle = globalAngle;
	_local = false;
}

void ServoPeriphery_t::SetLocalAngle(float localAngle) {
	_angle = localAngle;
	_local = true;
	_SetAngle(_angle);
}

void ServoPeriphery_t::Update() {
	if (_local) return;

	float pitch = _sensor_rotation->GetPitch();
	_SetAngle(_angle - pitch);
}

void ServoPeriphery_t::_SetAngle(float angle) {
	SetAngle((angle + (PI / 2)) * ((_maxVal - _minVal) / PI) + _minVal);
}

#pragma once

#include "SensorRotation.h"

class ServoPeriphery_t {
private:
	SensorRotation_t* _sensor_rotation;
	float _minVal;
	float _maxVal;
	float _angle;
	bool _local;
	void _SetAngle(float angle);
protected:
	virtual void SetAngle(float angle) = 0;
public:
	virtual ~ServoPeriphery_t() = default;
	ServoPeriphery_t(SensorRotation_t* sensor_rotation, float minVal = 0, float maxVal = 180);
	void SetMinVal(float minVal);
	void SetMaxVal(float maxVal);
	float GetAngle() const;
	void SetGlobalAngle(float globalAngle);
	void SetLocalAngle(float localAngle);
	void Update();
};
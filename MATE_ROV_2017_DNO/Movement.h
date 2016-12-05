#pragma once
#include "SensorManager.h"
#include "Motors.h"
#include "Regulators.h"

class Movement_t {
private:
	SensorManager_t* _sensor_manager;
	Motors_t* _motors;

	Regulators_t* _regulators;

	bool _use_regulators;
public:
	Movement_t(SensorManager_t* sensor_manager, Motors_t* motors, Regulators_t* regulators);

	void SetMotorThrust(int motorId, float thrust);

	void SetDepth(float depth);
	void SetVelocity(float velocityX, float velocityY, float velocityZ);

	void SetRotation(float angleY, float angleZ);
	void SetAngularVelocity(float angularVelocityY, float angularVelocityZ);

	void Stop();

	void Update() const;

	~Movement_t();
};


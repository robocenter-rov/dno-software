#pragma once
#include "SensorManager.h"
#include "Motors.h"
#include "Regulators.h"

class Movement_t {
private:
	SensorManager_t* _sensor_manager;
	Motors_t* _motors;
	MotorsForceDistributor_t* _motors_force_distributor;

	Regulators_t* _regulators;

	enum {
		USE_REGULATORS,
		USE_FORCE_DISTRIBUTOR,
		USE_MOTORS
	} _control_level_state;
public:
	Movement_t(SensorManager_t* sensor_manager, Motors_t* motors, MotorsForceDistributor_t* motors_force_distributor, Regulators_t* regulators);

	void SetMotorThrust(int motorId, float thrust);

	void SetDepth(float depth);
	void SetVelocity(float velocityX, float velocityY, float velocityZ);

	void SetRotation(float angleY, float angleZ);
	void SetAngularVelocity(float angularVelocityY, float angularVelocityZ);

	void SetLocalRotateForce(float angleY, float angleZ);
	void SetLocalMovementForce(float x, float y, float z);

	void Stop();

	void Update() const;

	~Movement_t();
};


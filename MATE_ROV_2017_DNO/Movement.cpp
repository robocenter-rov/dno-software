#include "Movement.h"

Movement_t::Movement_t(SensorManager_t* sensorManager, Motors_t* motors, Regulators_t* regulators) {
	_sensor_manager = sensorManager;
	_motors = motors;
	_regulators = regulators;
}

void Movement_t::SetMotorThrust(int motorId, float thrust) {
	_motors->SetMotorThrust(motorId, thrust);
	_use_regulators = false;
}

void Movement_t::SetDepth(float depth) {
	_regulators->SetTargetDepth(depth);
	_use_regulators = true;
}

void Movement_t::SetRotation(float angleY, float angleZ) {
	_regulators->SetRotation(angleY, angleZ);
	_use_regulators = true;
}

void Movement_t::SetAngularVelocity(float angularVelocityY, float angularVelocityZ) {
	_regulators->SetAngularVelocity(angularVelocityY, angularVelocityZ);
	_use_regulators = true;
}

void Movement_t::SetVelocity(float velocityX, float velocityY, float velocityZ) {
	_regulators->SetVelocity(velocityX, velocityY, velocityZ);
	_use_regulators = true;
}

void Movement_t::Stop() {
	_motors->StopMotors();
	_use_regulators = false;
}

void Movement_t::Update() const {
	if (_use_regulators) {
		_regulators->Update();
	}
}

Movement_t::~Movement_t() {}

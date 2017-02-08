#include "Movement.h"

Movement_t::Movement_t(Motors_t* motors, MotorsForceDistributor_t* motors_force_distributor, Regulators_t* regulators) {
	_motors = motors;
	_regulators = regulators;
	_motors_force_distributor = motors_force_distributor;
	_control_level_state = USE_MOTORS;
}

void Movement_t::SetMotorThrust(int motorId, float thrust) {
	_motors->SetMotorThrust(motorId, thrust);
	_control_level_state = USE_MOTORS;
}

void Movement_t::SetDepth(float depth) {
	_regulators->SetTargetDepth(depth);
	_control_level_state = USE_REGULATORS;
}

void Movement_t::SetRotation(float angleY, float angleZ) {
	_regulators->SetRotation(angleY, angleZ);
	_control_level_state = USE_REGULATORS;
}

void Movement_t::SetAngularVelocity(float angularVelocityY, float angularVelocityZ) {
	_regulators->SetAngularVelocity(angularVelocityY, angularVelocityZ);
	_control_level_state = USE_REGULATORS;
}

void Movement_t::SetLocalRotateForce(float angleY, float angleZ) {
	_motors_force_distributor->SetLocalRotateForce(angleY, angleZ);
	_control_level_state = USE_FORCE_DISTRIBUTOR;
}

void Movement_t::SetLocalMovementForce(float x, float y, float z) {
	_motors_force_distributor->SetLocalMoveForce(x, y, z);
	_control_level_state = USE_FORCE_DISTRIBUTOR;
}

void Movement_t::SetVelocity(float velocityX, float velocityY, float velocityZ) {
	_regulators->SetVelocity(velocityX, velocityY, velocityZ);
	_control_level_state = USE_REGULATORS;
}

void Movement_t::Stop() {
	_motors->StopMotors();
	_control_level_state = USE_MOTORS;
}

void Movement_t::Update() const {
	switch (_control_level_state) {
		case USE_REGULATORS: _regulators->Update(); break;
		case USE_FORCE_DISTRIBUTOR: _motors_force_distributor->Update(); break;
	}
}

Movement_t::~Movement_t() {}

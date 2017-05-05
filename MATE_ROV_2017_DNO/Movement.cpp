#include "Movement.h"

Movement_t::Movement_t(Motors_t* motors, MotorsForceDistributor_t* motors_force_distributor, AutoYaw_t* yaw_regulator, AutoDepth_t* depth_regulator) {
	_motors = motors;
	_motors_force_distributor = motors_force_distributor;
	_yaw_regulator = yaw_regulator;
	_depth_regulator = depth_regulator;
}

void Movement_t::SetMotorThrust(int motorId, float thrust) {
	_motors->SetMotorThrust(motorId, thrust);
	_use_auto_depth = _use_auto_pitch = _use_auto_yaw = false;
	_use_motors_direct = true;
}

float Movement_t::GetMotorThrust(int motorId) const {
	return _motors->GetMotorThrust(motorId);
}

void Movement_t::SetDepth(float depth) {
	_depth_regulator->SetDepth(depth);
	_use_auto_depth = true;
	_use_motors_direct = false;
}

void Movement_t::SetDepthForce(float depth) {
	_motors_force_distributor->AddGlobalMoveForce(0, 0, depth);
	_use_auto_depth = false;
	_use_motors_direct = false;
}

void Movement_t::SetYaw(float angleY) {
	_yaw_regulator->SetTarget(angleY);
	_use_auto_yaw = true;
	_use_motors_direct = false;
}

void Movement_t::SetYawForce(float angleY) {
	_motors_force_distributor->AddLocalRotateForce(angleY, 0);
	_use_auto_yaw = false;
	_use_motors_direct = false;
}

void Movement_t::SetPitch(float angleZ) {
	_pitch_regulator->SetTarget(angleZ);
	_use_auto_pitch = true;
	_use_motors_direct = false;
}

void Movement_t::SetPitchForce(float angleZ) {
	_motors_force_distributor->AddLocalRotateForce(0, angleZ);
	_use_auto_pitch = false;
	_use_motors_direct = false;
}

void Movement_t::SetLocalForce(float x, float y, float z) {
	_motors_force_distributor->SetLocalMoveForce(x, y, z);
	_use_motors_direct = false;
	_use_auto_depth = _use_auto_pitch = _use_auto_yaw = false;
}

void Movement_t::SetGlobalMoveForce(float x, float y, float z) {
	_motors_force_distributor->SetGlobalMoveForce(x, y, z);
	_use_motors_direct = false;
	_use_auto_depth = _use_auto_pitch = _use_auto_yaw = false;
}

void Movement_t::SetYawPid(float p, float i, float d) const {
	_yaw_regulator->SetPid(p, i, d);
}

void Movement_t::SetPitchPid(float p, float i, float d) const {
	_pitch_regulator->SetPid(p, i, d);
}

void Movement_t::SetDepthPid(float p, float i, float d) const {
	_depth_regulator->SetPid(p, i, d);
}

void Movement_t::Stop() {
	_motors->StopMotors();
	_use_motors_direct = false;
	_use_auto_depth = _use_auto_pitch = _use_auto_yaw = false;
}

void Movement_t::Update() const {
	if (!_use_motors_direct) {
		if (_use_auto_depth) {
			_depth_regulator->Update(_motors_force_distributor);
		}
		if (_use_auto_yaw) {
			_depth_regulator->Update(_motors_force_distributor);
		}
		if (_use_auto_pitch) {
			_depth_regulator->Update(_motors_force_distributor);
		}
		_motors_force_distributor->Update(_motors);
	}
}

Movement_t::~Movement_t() {}

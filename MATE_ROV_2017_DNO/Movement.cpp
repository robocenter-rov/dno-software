#include "Movement.h"

Movement_t::Movement_t(Motors_t* motors, MotorsForceDistributor_t* motors_force_distributor, AutoYaw_t* yaw_regulator, AutoPitch_t* pitch_regulator, AutoRoll_t* roll_regulator, AutoDepth_t* depth_regulator) {
	_motors = motors;
	_motors_force_distributor = motors_force_distributor;
	_yaw_regulator = yaw_regulator;
	_pitch_regulator = pitch_regulator;
	_roll_regulator = roll_regulator;
	_depth_regulator = depth_regulator;

	_local_x = _local_y = _local_z = 0;
	_global_x = _global_y = _global_z = 0;
	_local_yaw = _local_pitch = _local_roll = 0;

	_last_call_time = 0;
}

Motors_t* Movement_t::Motors() const {
	return _motors;
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
	_global_z = depth;
	_use_auto_depth = false;
	_use_motors_direct = false;
}

void Movement_t::SetYaw(float angleZ) {
	_yaw_regulator->SetTarget(angleZ);
	_use_auto_yaw = true;
	_use_motors_direct = false;
}

void Movement_t::SetYawForce(float angleZ) {
	_local_yaw = angleZ;
	_use_auto_yaw = false;
	_use_motors_direct = false;
}

void Movement_t::SetPitch(float angleY) {
	_pitch_regulator->SetTarget(angleY);
	_use_auto_pitch = true;
	_use_motors_direct = false;
}

void Movement_t::SetPitchForce(float angleY) {
	_local_pitch = angleY;
	_use_auto_pitch = false;
	_use_motors_direct = false;
}

void Movement_t::SetRoll(float angleY) {
	_roll_regulator->SetTarget(angleY);
	_use_auto_pitch = true;
	_use_motors_direct = false;
}

void Movement_t::SetRollForce(float angleY) {
	_local_roll = angleY;
	_use_auto_pitch = false;
	_use_motors_direct = false;
}

void Movement_t::SetLocalForce(float x, float y, float z) {
	_local_x = x;
	_local_y = y;
	_local_z = z;
	_use_motors_direct = false;
	_use_auto_depth = _use_auto_pitch = _use_auto_yaw = false;
}

void Movement_t::SetGlobalMoveForce(float x, float y, float z) {
	_global_x = x;
	_global_y = y;
	_global_z = z;
	_use_motors_direct = false;
	_use_auto_depth = _use_auto_pitch = _use_auto_yaw = false;
}

void Movement_t::SetYawPid(float p, float i, float d) const {
	_yaw_regulator->SetPid(p, i, d);
}

void Movement_t::SetPitchPid(float p, float i, float d) const {
	_pitch_regulator->SetPid(p, i, d);
}

void Movement_t::SetRollPid(float p, float i, float d) const {
	_roll_regulator->SetPid(p, i, d);
}

void Movement_t::SetDepthPid(float p, float i, float d) const {
	_depth_regulator->SetPid(p, i, d);
}

void Movement_t::GetYawPidState(float& in, float& target, float& out) const {
	_yaw_regulator->GetPidState(in, target, out);
}

void Movement_t::GetPitchPidState(float& in, float& target, float& out) const {
	_pitch_regulator->GetPidState(in, target, out);
}

void Movement_t::GetRollPidState(float& in, float& target, float& out) const {
	_roll_regulator->GetPidState(in, target, out);
}

void Movement_t::GetDepthPidState(float& in, float& target, float& out) const {
	_depth_regulator->GetPidState(in, target, out);
}

void Movement_t::Stop() {
	_motors->StopMotors();
	_use_motors_direct = true;
	_use_auto_depth = _use_auto_pitch = _use_auto_yaw = false;
}

void Movement_t::Update()
{
	if (!_use_motors_direct) {
		_motors_force_distributor->ClearForces();
		_motors_force_distributor->AddLocalMoveForce(_local_x, _local_y, _local_z);
		_motors_force_distributor->AddLocalRotateForce(_local_pitch, _local_roll, _local_yaw);
		//_motors_force_distributor->AddGlobalMoveForce(_global_x, _global_y, _global_z);
		if (millis() - _last_call_time >= 100) {
			_last_call_time = millis();
			if (_use_auto_depth) {
				_depth_regulator->Update(_motors_force_distributor);
			}
			if (_use_auto_yaw) {
				_yaw_regulator->Update(_motors_force_distributor);
			}
			if (_use_auto_pitch) {
				_pitch_regulator->Update(_motors_force_distributor);
			}
			if (_use_auto_roll) {
				_roll_regulator->Update(_motors_force_distributor);
			}
		}
		_motors_force_distributor->Update(_motors);
	}
}

Movement_t::~Movement_t() {}

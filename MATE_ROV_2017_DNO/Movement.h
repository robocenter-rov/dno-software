#pragma once
#include "Motors.h"
#include "MotorsForceDistributor.h"
#include "AutoYaw.h"
#include "DepthRegulator.h"
#include "AutoPitch.h"

class Movement_t {
private:
	Motors_t* _motors;
	MotorsForceDistributor_t* _motors_force_distributor;

	float _locax_x, _local_y, _local_z;
	float _global_x, _global_y, _global_z;
	float _local_yaw, _local_pitch;

	AutoYaw_t* _yaw_regulator;
	AutoPitch_t* _pitch_regulator;
	AutoDepth_t* _depth_regulator;

	bool _use_motors_direct = false;
	bool _use_auto_yaw = false;
	bool _use_auto_pitch = false;
	bool _use_auto_depth = false;
public:
	Movement_t(Motors_t* motors, MotorsForceDistributor_t* motors_force_distributor, AutoYaw_t* yaw_regulator, AutoPitch_t* pitch_regulator, AutoDepth_t* depth_regulator);

	Motors_t* Motors() const;

	void SetMotorThrust(int motorId, float thrust);
	float GetMotorThrust(int motorId) const;

	void SetDepth(float depth);
	void SetDepthForce(float depth);

	void SetYaw(float angleZ);
	void SetYawForce(float angleZ);

	void SetPitch(float angleY);
	void SetPitchForce(float angleY);

	void SetLocalForce(float x, float y, float z);
	void SetGlobalMoveForce(float x, float y, float z);

	void SetYawPid(float p, float i, float d) const;
	void SetPitchPid(float p, float i, float d) const;
	void SetDepthPid(float p, float i, float d) const;

	void GetYawPidState(float& in, float& target, float& out) const;
	void GetPitchPidState(float& in, float& target, float& out) const;
	void GetDepthPidState(float& in, float& target, float& out) const;

	void Stop();

	void Update() const;

	~Movement_t();
};


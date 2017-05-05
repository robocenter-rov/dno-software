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

	AutoYaw_t* _yaw_regulator;
	AutoPitch_t* _pitch_regulator;
	AutoDepth_t* _depth_regulator;

	bool _use_motors_direct = false;
	bool _use_auto_yaw = false;
	bool _use_auto_pitch = false;
	bool _use_auto_depth = false;
public:
	Movement_t(Motors_t* motors, MotorsForceDistributor_t* motors_force_distributor, AutoYaw_t* yaw_regulator, AutoDepth_t* depth_regulator);

	void SetMotorThrust(int motorId, float thrust);
	float GetMotorThrust(int motorId) const;

	void SetDepth(float depth);
	void SetDepthForce(float depth);

	void SetYaw(float angleY);
	void SetYawForce(float angleY);

	void SetPitch(float angleZ);
	void SetPitchForce(float angleZ);

	void SetLocalForce(float x, float y, float z);
	void SetGlobalMoveForce(float x, float y, float z);

	void SetYawPid(float p, float i, float d) const;
	void SetPitchPid(float p, float i, float d) const;
	void SetDepthPid(float p, float i, float d) const;

	void Stop();

	void Update() const;

	~Movement_t();
};


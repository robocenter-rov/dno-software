#pragma once
#include "Arduino.h"
#include "Motors.h"
#include "MathConstants.h"

class LocalMotorsForceDistributor_t {
private:
	float _x_move_force;
	float _y_move_force;
	float _z_move_force;

	float _y_rotate_force;
	float _z_rotate_force;
public:
	LocalMotorsForceDistributor_t() {
		_x_move_force = _y_move_force = _z_move_force = 0;
		_y_rotate_force = _z_rotate_force = 0;
	}

	void AddMoveForce(float x, float y, float z) {
		_x_move_force += x;
		_y_move_force += y;
		_z_move_force += z;
	}

	void AddRotateForce(float y, float z) {
		_y_rotate_force += y;
		_z_rotate_force += z;
	}

	void SetXMoveForce(float x) {
		_x_move_force = x;
	}

	void SetYMoveForce(float y) {
		_y_move_force = y;
	}

	void SetZMoveForce(float z) {
		_z_move_force = z;
	}

	void SetMoveForce(float x, float y, float z) {
		_x_move_force = x;
		_y_move_force = y;
		_z_move_force = z;
	}

	void SetRotateForce(float y, float z) {
		_y_rotate_force = y;
		_z_rotate_force = z;
	}

	void SetYRotateForce(float y) {
		_y_rotate_force = y;
	}

	void SetZRotateForce(float z) {
		_z_rotate_force = z;
	}

	void ClearForces() {
		_x_move_force = _y_move_force = _z_move_force = 0;
		_y_rotate_force = _z_rotate_force = 0;
	}

	void Update(Motors_t* motors) const {
		float frontLeftMotorThrust =
			(_x_move_force / 4) * SQRT2 +
			(_y_move_force / 4) * SQRT2 - _z_rotate_force;

		float frontRightMotorThrust =
			-(_x_move_force / 4) * SQRT2 +
			(_y_move_force / 4) * SQRT2 + _z_rotate_force;

		float backLeftMotorThrust =
			(_x_move_force / 4) * SQRT2 +
			-(_y_move_force / 4) * SQRT2 + _z_rotate_force;

		float backRightMotorThrust =
			-(_x_move_force / 4) * SQRT2 +
			-(_y_move_force / 4) * SQRT2 - _z_rotate_force;
		
		float frontMotorThrust = _z_move_force / 2 + _y_rotate_force / 2;
		float backMotorThrust = _z_move_force / 2 - _y_rotate_force / 2;

		float motorsThrust[6] = {
			frontLeftMotorThrust,
			frontRightMotorThrust,
			backLeftMotorThrust,
			backRightMotorThrust,
			frontMotorThrust,
			backMotorThrust
		};

		float maxMotorThrust = 0;

		for (int i = 0; i < 6; i++) {
			if (abs(motorsThrust[i]) > maxMotorThrust) {
				maxMotorThrust = motorsThrust[i];
			}
		}

		if (maxMotorThrust > 1) {
			for (int i = 0; i < 6; i++) {
				motorsThrust[i] /= maxMotorThrust;
			}
		}

		motors->SetMotorsThrust(motorsThrust);
	}
};

#pragma once
#include "Motor.h"

class Motors_t {
private:
	Motor_t* _motors[6];
	int _pos[6];
public:
	Motors_t(Motor_t* front_left_motor, Motor_t* front_right_motor, Motor_t* back_left_motor, Motor_t* back_right_motor, Motor_t* front_motor, Motor_t* back_motor) {
		_motors[0] = front_left_motor;
		_motors[1] = front_right_motor;
		_motors[2] = back_left_motor;
		_motors[3] = back_right_motor;
		_motors[4] = front_motor;
		_motors[5] = back_motor;

		for (int i = 0; i < 6; i++) {
			_pos[i] = i;
		}
	}

	void SetMotorThrust(int motor_id, float thrust) const {
		_motors[_pos[motor_id]]->SetThrust(thrust);
	}

	float GetMotorThrust(int motor_id) const {
		return _motors[_pos[motor_id]]->GetThrust();
	}

	void SetMotorsThrust(float* thrust) const {
		for (int i = 0; i < 6; i++) {
			_motors[_pos[i]]->SetThrust(thrust[i]);
		}
	}

	void SetMotorsPositions(int m1pos, int m2pos, int m3pos, int m4pos, int m5pos, int m6pos) {
		_pos[0] = m1pos;
		_pos[1] = m2pos;
		_pos[2] = m3pos;
		_pos[3] = m4pos;
		_pos[4] = m5pos;
		_pos[5] = m6pos;
	}

	void SetMotorsMultipliers(float m1mul, float m2mul, float m3mul, float m4mul, float m5mul, float m6mul) {
		_motors[_pos[0]]->SetMultiplier(m1mul);
		_motors[_pos[1]]->SetMultiplier(m2mul);
		_motors[_pos[2]]->SetMultiplier(m3mul);
		_motors[_pos[3]]->SetMultiplier(m4mul);
		_motors[_pos[4]]->SetMultiplier(m5mul);
		_motors[_pos[5]]->SetMultiplier(m6mul);
	}

	void SetFrontMotorThrust(float thrust) const {
		SetMotorThrust(_pos[0], thrust);
	}

	void SetBackMotorThrust(float thrust) const {
		SetMotorThrust(_pos[1], thrust);
	}

	void SetLeftFrontMotorThrust(float thrust) const {
		SetMotorThrust(_pos[2], thrust);
	}

	void SetRightFrontMotorThrust(float thrust) const {
		SetMotorThrust(_pos[3], thrust);
	}

	void SetLeftBackMotorThrust(float thrust) const {
		SetMotorThrust(_pos[4], thrust);
	}

	void SetRightBackMotorThrust(float thrust) const {
		SetMotorThrust(_pos[5], thrust);
	}

	void StopMotors() const {
		for (int i = 0; i < 6; i++) {
			_motors[i]->SetThrust(0);
		}
	}
};

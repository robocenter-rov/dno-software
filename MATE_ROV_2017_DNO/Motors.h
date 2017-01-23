#pragma once
#include "Motor.h"

class Motors_t {
private:
	int _motor_count;
	Motor_t** _motors;
public:
	Motors_t(int motor_count) {
		_motor_count = motor_count;
		_motors = new Motor_t*[motor_count];
	}

	void AddMotor(Motor_t* motor) const {
		static int current_motor_id = 0;

		_motors[current_motor_id++] = motor;
	}

	void SetMotorThrust(int motor_id, float thrust) const {
		_motors[motor_id]->SetThrust(thrust);
	}

	void SetMotorsThrust(float* thrust) const {
		for (int i = 0; i < _motor_count; i++) {
			_motors[i]->SetThrust(thrust[i]);
		}
	}

	void SetFrontMotorThrust(float thrust) const {
		SetMotorThrust(0, thrust);
	}

	void SetBackMotorThrust(float thrust) const {
		SetMotorThrust(1, thrust);
	}

	void SetLeftFrontMotorThrust(float thrust) const {
		SetMotorThrust(2, thrust);
	}

	void SetRightFrontMotorThrust(float thrust) const {
		SetMotorThrust(3, thrust);
	}

	void SetLeftBackMotorThrust(float thrust) const {
		SetMotorThrust(4, thrust);
	}

	void SetRightBackMotorThrust(float thrust) const {
		SetMotorThrust(5, thrust);
	}

	void StopMotors() const {
		for (int i = 0; i < _motor_count; i++) {
			_motors[i]->SetThrust(0);
		}
	}
};

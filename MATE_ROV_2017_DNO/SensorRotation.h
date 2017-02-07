#pragma once

class SensorRotation_t {
public:
	struct Data_t {
		float q0;
		float q1;
		float q2;
		float q3;
		float AngleVelocityX;
		float AngleVelocityY;
		float AngleVelocityZ;

		Data_t() {
			q0 = q1 = q2 = q3 = AngleVelocityX = AngleVelocityY = AngleVelocityZ = 0;
		}

		Data_t(float q0, float q1, float q2, float q3, float v_x, float v_y, float v_z) : q0(q0), q1(q1), q2(q2), q3(q3) {
			AngleVelocityX = v_x;
			AngleVelocityY = v_y;
			AngleVelocityZ = v_z;
		}
	};

	SensorRotation_t();

	Data_t GetRotation();

	~SensorRotation_t();
};


#pragma once

class SensorRotation_t {
public:
	struct Data_t {
		float RotationX;
		float RotationY;
		float RotationZ;
		float AngleVelocityX;
		float AngleVelocityY;
		float AngleVelocityZ;

		Data_t(float r_x, float r_y, float r_z, float v_x, float v_y, float v_z) {
			RotationX = r_x;
			RotationY = r_y;
			RotationZ = r_z;

			AngleVelocityX = v_x;
			AngleVelocityY = v_y;
			AngleVelocityZ = v_z;
		}
	};

	SensorRotation_t();

	Data_t GetRotation();

	~SensorRotation_t();
};


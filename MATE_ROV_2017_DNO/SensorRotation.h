#pragma once
#include "Libs/adxl345driver/ADXL345.h";
#include "Libs/ITG3200_library/ITG3200.h";
#include "Libs/HMC58X3/HMC58X3.h";

class SensorRotation_t {
private:
	ADXL345 _accel;
	ITG3200 _gyro;
	HMC58X3 _magn;

	float _twoKp = 2.0f * 0.5f;
	float _twoKi = 2.0f * 0.1f;

	float _q0 = 1.0f, _q1 = 0.0f, _q2 = 0.0f, _q3 = 0.0f;
	float _sampleFreq;
	float _integralFBx = 0.0f, _integralFBy = 0.0f, _integralFBz = 0.0f;

	float _gyro_bias[3];
	float _gyro_scale;

	float _accel_bias[3];
	float _accel_scale[3];

	float _magn_cal_matrix[3][3];
	float _magn_bias[3];

	float _beta = 0.02f;

	float _pitch;
	float _roll;
	float _yaw;

	float _gx, _gy, _gz;

	bool _pitch_updated;
	bool _roll_updated;
	bool _yaw_updated;
	bool _ypr_calc_cache_updated;

	unsigned long int _lastUpdate = 0;
	
	void UpdateYprCalcCache();

	void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
	void MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz);
	void MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az);
public:
	SensorRotation_t();

	void GetRotation(float& q0, float& q1, float& q2, float& q3) const;
	void GetRawData(int* raw_data);
	void GetCalibratedData(float* calibrated_data);

	void SetCalibrationValues(const float gyro_bias[3], const float gyro_scale, 
		const float accel_bias[3], const float accel_scale[3], const float magn_cal_matrix[3][3], const float magn_bias[3]);

	void GyroZeroCalibrate(unsigned int totSamples, unsigned int sampleDelayMS);

	void Update();

	float GetPitch();
	float GetRoll();
	float GetYaw();

	~SensorRotation_t();
};


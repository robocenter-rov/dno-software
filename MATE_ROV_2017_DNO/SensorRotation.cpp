#include "SensorRotation.h"
#include "Arduino.h"

static float invSqrt(float x) {
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

void SensorRotation_t::MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az) {
	float recipNorm;
	float halfvx, halfvy, halfvz;
	float halfex, halfey, halfez;
	float qa, qb, qc;

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		// Estimated direction of gravity and vector perpendicular to magnetic flux
		halfvx = _q1 * _q3 - _q0 * _q2;
		halfvy = _q0 * _q1 + _q2 * _q3;
		halfvz = _q0 * _q0 - 0.5f + _q3 * _q3;

		// Error is sum of cross product between estimated and measured direction of gravity
		halfex = (ay * halfvz - az * halfvy);
		halfey = (az * halfvx - ax * halfvz);
		halfez = (ax * halfvy - ay * halfvx);

		// Compute and apply integral feedback if enabled
		if (_twoKi > 0.0f) {
			_integralFBx += _twoKi * halfex * (1.0f / _sampleFreq);  // integral error scaled by Ki
			_integralFBy += _twoKi * halfey * (1.0f / _sampleFreq);
			_integralFBz += _twoKi * halfez * (1.0f / _sampleFreq);
			gx += _integralFBx;  // apply integral feedback
			gy += _integralFBy;
			gz += _integralFBz;
		}
		else {
			_integralFBx = 0.0f; // prevent integral windup
			_integralFBy = 0.0f;
			_integralFBz = 0.0f;
		}

		// Apply proportional feedback
		gx += _twoKp * halfex;
		gy += _twoKp * halfey;
		gz += _twoKp * halfez;
	}

	// Integrate rate of change of quaternion
	gx *= (0.5f * (1.0f / _sampleFreq));   // pre-multiply common factors
	gy *= (0.5f * (1.0f / _sampleFreq));
	gz *= (0.5f * (1.0f / _sampleFreq));
	qa = _q0;
	qb = _q1;
	qc = _q2;
	_q0 += (-qb * gx - qc * gy - _q3 * gz);
	_q1 += (qa * gx + qc * gz - _q3 * gy);
	_q2 += (qa * gy - qb * gz + _q3 * gx);
	_q3 += (qa * gz + qb * gy - qc * gx);

	// Normalise quaternion
	recipNorm = invSqrt(_q0 * _q0 + _q1 * _q1 + _q2 * _q2 + _q3 * _q3);
	_q0 *= recipNorm;
	_q1 *= recipNorm;
	_q2 *= recipNorm;
	_q3 *= recipNorm;
}

void SensorRotation_t::MadgwickAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) {
	float recipNorm;
	float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
	float halfex = 0.0f, halfey = 0.0f, halfez = 0.0f;
	float qa, qb, qc;

	// Auxiliary variables to avoid repeated arithmetic
	q0q0 = _q0 * _q0;
	q0q1 = _q0 * _q1;
	q0q2 = _q0 * _q2;
	q0q3 = _q0 * _q3;
	q1q1 = _q1 * _q1;
	q1q2 = _q1 * _q2;
	q1q3 = _q1 * _q3;
	q2q2 = _q2 * _q2;
	q2q3 = _q2 * _q3;
	q3q3 = _q3 * _q3;

	// Use magnetometer measurement only when valid (avoids NaN in magnetometer normalisation)
	if ((mx != 0.0f) && (my != 0.0f) && (mz != 0.0f)) {
		float hx, hy, bx, bz;
		float halfwx, halfwy, halfwz;

		// Normalise magnetometer measurement
		recipNorm = invSqrt(mx * mx + my * my + mz * mz);
		mx *= recipNorm;
		my *= recipNorm;
		mz *= recipNorm;

		// Reference direction of Earth's magnetic field
		hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
		hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
		bx = sqrt(hx * hx + hy * hy);
		bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));

		// Estimated direction of magnetic field
		halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
		halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
		halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);

		// Error is sum of cross product between estimated direction and measured direction of field vectors
		halfex = (my * halfwz - mz * halfwy);
		halfey = (mz * halfwx - mx * halfwz);
		halfez = (mx * halfwy - my * halfwx);
	}

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if ((ax != 0.0f) && (ay != 0.0f) && (az != 0.0f)) {
		float halfvx, halfvy, halfvz;

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		// Estimated direction of gravity
		halfvx = q1q3 - q0q2;
		halfvy = q0q1 + q2q3;
		halfvz = q0q0 - 0.5f + q3q3;

		// Error is sum of cross product between estimated direction and measured direction of field vectors
		halfex += (ay * halfvz - az * halfvy);
		halfey += (az * halfvx - ax * halfvz);
		halfez += (ax * halfvy - ay * halfvx);
	}
	// Apply feedback only when valid data has been gathered from the accelerometer or magnetometer
	if (halfex != 0.0f && halfey != 0.0f && halfez != 0.0f) {
		// Compute and apply integral feedback if enabled
		if (_twoKi > 0.0f) {
			_integralFBx += _twoKi * halfex * (1.0f / _sampleFreq);  // integral error scaled by Ki
			_integralFBy += _twoKi * halfey * (1.0f / _sampleFreq);
			_integralFBz += _twoKi * halfez * (1.0f / _sampleFreq);
			gx += _integralFBx;  // apply integral feedback
			gy += _integralFBy;
			gz += _integralFBz;
		}
		else {
			_integralFBx = 0.0f; // prevent integral windup
			_integralFBy = 0.0f;
			_integralFBz = 0.0f;
		}

		// Apply proportional feedback
		gx += _twoKp * halfex;
		gy += _twoKp * halfey;
		gz += _twoKp * halfez;
	}

	// Integrate rate of change of quaternion
	gx *= (0.5f * (1.0f / _sampleFreq));   // pre-multiply common factors
	gy *= (0.5f * (1.0f / _sampleFreq));
	gz *= (0.5f * (1.0f / _sampleFreq));
	qa = _q0;
	qb = _q1;
	qc = _q2;
	_q0 += (-qb * gx - qc * gy - _q3 * gz);
	_q1 += (qa * gx + qc * gz - _q3 * gy);
	_q2 += (qa * gy - qb * gz + _q3 * gx);
	_q3 += (qa * gz + qb * gy - qc * gx);

	// Normalise quaternion
	recipNorm = invSqrt(_q0 * _q0 + _q1 * _q1 + _q2 * _q2 + _q3 * _q3);
	_q0 *= recipNorm;
	_q1 *= recipNorm;
	_q2 *= recipNorm;
	_q3 *= recipNorm;
}

void SensorRotation_t::MadgwickAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az) {
	float recipNorm;
	float s0, s1, s2, s3;
	float qDot1, qDot2, qDot3, qDot4;
	float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2, _8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

	// Rate of change of quaternion from gyroscope
	qDot1 = 0.5f * (-_q1 * gx - _q2 * gy - _q3 * gz);
	qDot2 = 0.5f * (_q0 * gx + _q2 * gz - _q3 * gy);
	qDot3 = 0.5f * (_q0 * gy - _q1 * gz + _q3 * gx);
	qDot4 = 0.5f * (_q0 * gz + _q1 * gy - _q2 * gx);

	// Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
	if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

		// Normalise accelerometer measurement
		recipNorm = invSqrt(ax * ax + ay * ay + az * az);
		ax *= recipNorm;
		ay *= recipNorm;
		az *= recipNorm;

		// Auxiliary variables to avoid repeated arithmetic
		_2q0 = 2.0f * _q0;
		_2q1 = 2.0f * _q1;
		_2q2 = 2.0f * _q2;
		_2q3 = 2.0f * _q3;
		_4q0 = 4.0f * _q0;
		_4q1 = 4.0f * _q1;
		_4q2 = 4.0f * _q2;
		_8q1 = 8.0f * _q1;
		_8q2 = 8.0f * _q2;
		q0q0 = _q0 * _q0;
		q1q1 = _q1 * _q1;
		q2q2 = _q2 * _q2;
		q3q3 = _q3 * _q3;

		// Gradient decent algorithm corrective step
		s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
		s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * _q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
		s2 = 4.0f * q0q0 * _q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
		s3 = 4.0f * q1q1 * _q3 - _2q1 * ax + 4.0f * q2q2 * _q3 - _2q2 * ay;
		recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
		s0 *= recipNorm;
		s1 *= recipNorm;
		s2 *= recipNorm;
		s3 *= recipNorm;

		// Apply feedback step
		qDot1 -= _beta * s0;
		qDot2 -= _beta * s1;
		qDot3 -= _beta * s2;
		qDot4 -= _beta * s3;
	}

	// Integrate rate of change of quaternion to yield quaternion
	_q0 += qDot1 * (1.0f / _sampleFreq);
	_q1 += qDot2 * (1.0f / _sampleFreq);
	_q2 += qDot3 * (1.0f / _sampleFreq);
	_q3 += qDot4 * (1.0f / _sampleFreq);

	// Normalise quaternion
	recipNorm = invSqrt(_q0 * _q0 + _q1 * _q1 + _q2 * _q2 + _q3 * _q3);
	_q0 *= recipNorm;
	_q1 *= recipNorm;
	_q2 *= recipNorm;
	_q3 *= recipNorm;
}

SensorRotation_t::SensorRotation_t()
{
	delay(5);
	_accel.init(ADXL345_ADDR_ALT_LOW);
	_accel.set_bw(ADXL345_BW_12);

	_gyro.init(ITG3200_ADDR_AD0_LOW);

	_magn.init(true);
}

void SensorRotation_t::GetRotation(float& q0, float& q1, float& q2, float& q3) const {
	q0 = _q0;
	q1 = _q1;
	q2 = _q2;
	q3 = _q3;
}

void SensorRotation_t::GetRawData(int* raw_data) {
	_accel.readAccel(raw_data, raw_data + 1, raw_data + 2);
	_gyro.readGyroRaw(raw_data + 3, raw_data + 4, raw_data + 5);
	_magn.getRaw(raw_data + 6, raw_data + 7, raw_data + 8);
}

void SensorRotation_t::GetCalibratedData(float* calibrated_data) {
	int rawData[9];
	GetRawData(rawData);
	
	float cal_accel[3];
	for (int i = 0; i < 3; i++) {
		cal_accel[i] = (rawData[i] - _accel_bias[i]) / _accel_scale[i];
	}

	float cal_gyro[3];
	for (int i = 0; i < 3; i++) {
		cal_gyro[i] = (rawData[i + 3] - _gyro_bias[i]) / _gyro_scale;
	}

	float t_magn[3];
	for (int i = 0; i < 3; i++) t_magn[i] = rawData[i + 6] - _magn_bias[i];
	float cal_magn[3] = { 0, 0, 0 };
	for (int i = 0; i<3; ++i)
		for (int j = 0; j < 3; ++j)
			cal_magn[i] += _magn_cal_matrix[i][j] * t_magn[j];

	memcpy(calibrated_data, cal_accel, sizeof cal_accel);
	memcpy(calibrated_data + 3, cal_gyro, sizeof cal_gyro);
	memcpy(calibrated_data + 6, cal_magn, sizeof cal_magn);
}

void SensorRotation_t::SetCalibrationValues(const float gyro_bias[3], const float gyro_scale, 
	const float accel_bias[3], const float accel_scale[3], const float magn_cal_matrix[3][3], const float magn_bias[3]) 
{
	memcpy(_gyro_bias, gyro_bias, sizeof _gyro_bias);
	_gyro_scale = gyro_scale;
	memcpy(_accel_bias, accel_bias, sizeof _accel_bias);
	memcpy(_accel_scale, accel_scale, sizeof _accel_scale);
	memcpy(_magn_cal_matrix, magn_cal_matrix, sizeof _magn_cal_matrix);
	memcpy(_magn_bias, magn_bias, sizeof _magn_bias);
}

void SensorRotation_t::Update() {
	unsigned long int now = micros();
	_sampleFreq = 1.0 / ((now - _lastUpdate) / 1000000.0);
	_lastUpdate = now;

	float data[9];
	GetCalibratedData(data);
	MadgwickAHRSupdateIMU(data[3], data[4], data[5], data[0], data[1], data[2]);
}

SensorRotation_t::~SensorRotation_t() {}

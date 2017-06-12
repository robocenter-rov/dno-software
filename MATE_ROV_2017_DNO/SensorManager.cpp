#include "SensorManager.h"

SensorManager_t::SensorManager_t(SensorRotation_t* sensor_rotation, SensorDepth_t* sensor_depth) {
	_sensorRotation = sensor_rotation;
	_sensorDepth = sensor_depth;
}

void SensorManager_t::SetCalibrationValues(const float gyro_bias[3], const float gyro_scale, const float accel_bias[3], const float accel_scale[3], const float magn_cal_matrix[3][3], const float magn_bias[3]) {
	_sensorRotation->SetCalibrationValues(gyro_bias, gyro_scale, accel_bias, accel_scale, magn_cal_matrix, magn_bias);
}

void SensorManager_t::GetRotation(float& q1, float& q2, float& q3, float& q4) {
	_sensorRotation->GetRotation(q1, q2, q3, q4);
}

void SensorManager_t::GetRawRotation(int* raw_data) {
	_sensorRotation->GetRawData(raw_data);
}

void SensorManager_t::GetCalibratedRotation(float* calibrated_data) {
	_sensorRotation->GetCalibratedData(calibrated_data);
}

float SensorManager_t::GetDepth() {
	return _sensorDepth->GetDepth();
}

float SensorManager_t::GetRawDepth() {
	return _sensorDepth->GetRawData();
}

SensorRotation_t* SensorManager_t::Rotation() {
	return _sensorRotation;
}

void SensorManager_t::Update() const {
	_sensorRotation->Update();
}

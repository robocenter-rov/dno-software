#pragma once

#include "FlashlightPeriphery.h"
#include "ManipulatorPeriphery.h"
#include "BluetoothPeriphery.h"
#include "PCA9685ServoMotor.h"

class PeripheryManager_t {
private:
	FlashlightPeriphery_t* _flashlight;
	ManipulatorPeriphery_t* _manipulator;
	BluetoothPeriphery_t* _bluetooth;
	PCA9685ServoMotor_t* _camera1;
	PCA9685ServoMotor_t* _camera2;
	PCA9685ServoMotor_t* _m1;
	PCA9685ServoMotor_t* _m2;

public:
	PeripheryManager_t(FlashlightPeriphery_t* flashlight, ManipulatorPeriphery_t* manipulator,
		BluetoothPeriphery_t* bluetooth, PCA9685ServoMotor_t* camera1, PCA9685ServoMotor_t* camera2,
		PCA9685ServoMotor_t* m1, PCA9685ServoMotor_t* m2) {
		_flashlight = flashlight;
		_manipulator = manipulator;
		_bluetooth = bluetooth;
		_camera1 = camera1;
		_camera2 = camera2;
		_m1 = m1;
		_m2 = m2;
	}

	void TurnOnFlashlight() const {
		_flashlight->TurnOn();
	}

	void TurnOffFlashlight() const {
		_flashlight->TurnOff();
	}

	void SetFlashlightState(bool state) const {
		_flashlight->SetState(state);
	}

	bool ReceiveBluetoothMessage(char* input_buffer) const
	{
		return _bluetooth->ReceiveBluetoothMessage(input_buffer);
	}

	bool GetFlashlightState() const {
		return _flashlight->GetState();
	}

	ManipulatorPeriphery_t* GetManipulator() const {
		return _manipulator;
	}

	void SetAngleCamera1(float angle_radians) const
	{
		_camera1->SetAngle(angle_radians);
	}

	void SetAngleCamera2(float angle_radians) const
	{
		_camera2->SetAngle(angle_radians);
	}

	void SetAngleM1(float angle_radians) const
	{
		_m1->SetAngle(angle_radians);
	}

	void SetAngleM2(float angle_radians) const
	{
		_m2->SetAngle(angle_radians);
	}

	void SetOffsetCamera1(float offset) const { _camera1->SetOffset(offset); }

	void SetOffsetCamera2(float offset) const { _camera2->SetOffset(offset); }

	void SetDirectionCamera1(bool direction) const { _camera1->SetDirection(direction); }
	void SetDirectionCamera2(bool direction) const { _camera2->SetDirection(direction); }

	float GetAngleCamera1() const { return _camera1->GetAngle(); }
	float GetAngleCamera2() const { return _camera2->GetAngle(); }
};

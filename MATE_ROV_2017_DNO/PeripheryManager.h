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

public:
	PeripheryManager_t(FlashlightPeriphery_t* flashlight, ManipulatorPeriphery_t* manipulator,
		BluetoothPeriphery_t* bluetooth, PCA9685ServoMotor_t* camera1, PCA9685ServoMotor_t* camera2) {
		_flashlight = flashlight;
		_manipulator = manipulator;
		_bluetooth = bluetooth;
		_camera1 = camera1;
		_camera2 = camera2;
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

	void ReceiveBluetoothMessage(bool state) const
	{
		_bluetooth->ReceiveBluetoothMessage();
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
};

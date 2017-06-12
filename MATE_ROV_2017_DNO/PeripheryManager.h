#pragma once

#include "FlashlightPeriphery.h"
#include "ManipulatorPeriphery.h"
#include "BluetoothPeriphery.h"
#include "PCA9685ServoMotor.h"
#include "ArduinoServoMotor.h"

class PeripheryManager_t {
private:
	FlashlightPeriphery_t* _flashlight;
	ManipulatorPeriphery_t* _manipulator;
	BluetoothPeriphery_t* _bluetooth;
	ArduinoServoMotor_t* _camera1;
	ArduinoServoMotor_t* _camera2;
	PCA9685ServoMotor_t* _m1;
	PCA9685ServoMotor_t* _m2;

public:
	PeripheryManager_t(FlashlightPeriphery_t* flashlight, ManipulatorPeriphery_t* manipulator,
		BluetoothPeriphery_t* bluetooth, ArduinoServoMotor_t* camera1, ArduinoServoMotor_t* camera2,
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

	void SetLocalAngleCamera1(float angle_radians) const
	{
		_camera1->SetLocalAngle(angle_radians);
	}

	void SetLocalAngleCamera2(float angle_radians) const
	{
		_camera2->SetLocalAngle(angle_radians);
	}

	void SetGlobalAngleCamera2(float angle_radians) const
	{
		_camera2->SetGlobalAngle(angle_radians);
	}

	void SetGlobalAngleCamera1(float angle_radians) const
	{
		_camera1->SetGlobalAngle(angle_radians);
	}

	void SetAngleM1(float angle_radians) const
	{
		_m1->SetLocalAngle(angle_radians);
	}

	void SetAngleM2(float angle_radians) const
	{
		_m2->SetLocalAngle(angle_radians);
	}

	void SetCam1MinVal(float val) const { _camera1->SetMinVal(val); }
	void SetCam1MaxVal(float val) const { _camera1->SetMaxVal(val); }
	void SetCam2MinVal(float val) const { _camera2->SetMinVal(val); }
	void SetCam2MaxVal(float val) const { _camera2->SetMaxVal(val); }

	float GetAngleCamera1() const { return _camera1->GetAngle(); }
	float GetAngleCamera2() const { return _camera2->GetAngle(); }

	void Update() const {
		_camera1->Update();
		_camera2->Update();
		_m1->Update();
		_m2->Update();
	};
};

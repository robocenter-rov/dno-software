#pragma once

#include "FlashlightPeriphery.h"

class PeripheryManager_t {
private:
	FlashlightPeriphery_t* _flashlight;
public:
	PeripheryManager_t(FlashlightPeriphery_t* flashlight) {
		_flashlight = flashlight;
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

	bool GetFlashlightState() const {
		return _flashlight->GetState();
	}
};
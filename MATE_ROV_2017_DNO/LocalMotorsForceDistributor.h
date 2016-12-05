#pragma once
#include "Motors.h"

class LocalMotorsForceDistributor_t {
private:
	Motors_t* _motors;
public:
	LocalMotorsForceDistributor_t(Motors_t* motors) {
		_motors = motors;
	}

	void AddMoveForce(float x, float y, float z) {
		
	}

	void AddRotateForce(float y, float z) {
		
	}
};

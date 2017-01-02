#pragma once

#include "Main.h"
#include "Motor.h"

class DebugMotorsMain_t : public Main_t {
private:
	Motor_t* _motor;
public:
	DebugMotorsMain_t(Motor_t* motor);

	void Loop() override;
};

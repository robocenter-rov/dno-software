#pragma once

#include "Main\Main.h"
#include "Periphery\PeripheryManager.h"
#include "Movement\Movement.h"

class TestPCAMain_t : public Main_t {
private:
	PeripheryManager_t* _periphery_manager;
	Movement_t* _movement;
public:
	TestPCAMain_t(PeripheryManager_t* periphery_manager, Movement_t* movement);

	int Begin() override;
	void Loop() override;
};

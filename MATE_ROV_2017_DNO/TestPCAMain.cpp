#include "TestPCAMain.h"

TestPCAMain_t::TestPCAMain_t(PeripheryManager_t* periphery_manager, Movement_t* movement) {
	_periphery_manager = periphery_manager;
	_movement = movement;
}

int TestPCAMain_t::Begin() { return 0; }

void TestPCAMain_t::Loop() {
	float angle = millis() / 1000.f * PI;

	_periphery_manager->GetManipulator()->SetArmAngle(abs(sin(millis() / 1000.f * PI)));
	_periphery_manager->GetManipulator()->SetHandAngle(abs(sin(millis() / 1000.f * PI + PI / 2)));

	for (int i = 0; i < 6; i++) {
		_movement->SetMotorThrust(i, sin(millis() / 1000.f * PI + PI / 6 * i));
	}
}

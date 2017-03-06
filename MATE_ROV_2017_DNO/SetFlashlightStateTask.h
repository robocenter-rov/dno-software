#pragma once

#include "Task.h"

class SetFlashlightStateTask_t : public Task_t {
private:
	bool _flashlight_state;
	PeripheryManager_t* _periphery_manager;
	ResourceLocker::AutoLocker_t _locker;
public:
	SetFlashlightStateTask_t(int id, bool state, PeripheryManager_t* periphery_manager);
	bool LockNeededResources(RESOURCE& out_locked_resource, int worker_id) override;
	bool UpdateState(SelfExpandoContainer_t<TaskState_t>& out_state) override;
};

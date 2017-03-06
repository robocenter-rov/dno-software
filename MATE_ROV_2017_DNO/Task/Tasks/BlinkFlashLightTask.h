#pragma once

#include "../../Task/Task.h"

class BlinkFlashlightTask_t : public Task_t {
private:
	int _blink_count;
	int _change_count;
	unsigned long _last_change_time;
	PeripheryManager_t* _periphery_manager;
	ResourceLocker::AutoLocker_t _locker;
public:
	BlinkFlashlightTask_t(int id, int blink_count, PeripheryManager_t* periphery_manager);
	bool LockNeededResources(RESOURCE& out_locked_resource, int worker_id) override;
	bool UpdateState(SelfExpandoContainer_t<TaskState_t>& out_state) override;
	~BlinkFlashlightTask_t() override;
};

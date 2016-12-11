#pragma once

#include "PeripheryManager.h"
#include "ResourceLocker.h"
#include "TaskState.h"

class Task_t {
protected:
	unsigned int _id;
	unsigned int _worker_id;
public:
	Task_t(unsigned int id) : _id(id), _worker_id(0) {}
	virtual ~Task_t() {}
	virtual bool LockNeededResources(RESOURCE& out_locked_resource) { return true; }
	virtual bool UpdateState(TaskState_t* out_state) { return true; }
	unsigned int GetId() const { return _id; }
	void SetWorkerId(unsigned int worker_id);
};

class SetFlashlightStateTask_t : public Task_t {
private:
	bool _flashlight_state;
	PeripheryManager_t* _periphery_manager;
	ResourceLocker::AutoLocker_t _locker;
public:
	SetFlashlightStateTask_t(int id, bool state, PeripheryManager_t* periphery_manager);
	bool LockNeededResources(RESOURCE& out_locked_resource) override;
	bool UpdateState(TaskState_t* out_state) override;
};

class BlinkFlashlightTask_t : public Task_t {
private:
	int _blink_count;
	int _change_count;
	unsigned long _last_change_time;
	PeripheryManager_t* _periphery_manager;
	ResourceLocker::AutoLocker_t _locker;
public:
	BlinkFlashlightTask_t(int id, int blink_count, PeripheryManager_t* periphery_manager);
	bool LockNeededResources(RESOURCE& out_locked_resource) override;
	bool UpdateState(TaskState_t* out_state) override;
	~BlinkFlashlightTask_t() override;
};

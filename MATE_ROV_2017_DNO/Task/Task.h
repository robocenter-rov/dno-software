#pragma once

#include "../Periphery/PeripheryManager.h"
#include "../ResourceLocker/ResourceLocker.h"
#include "../Utils/SelfExpandoContainer.h"
#include "TaskState.h"

class Task_t {
protected:
	unsigned int _tag;
public:
	Task_t(unsigned int tag) : _tag(tag) {}
	virtual ~Task_t() {}
	//awake();
	//virtual bool destroy() {} 
	virtual bool LockNeededResources(RESOURCE& out_locked_resource, int worker_id) { return true; }
	virtual bool UpdateState(SelfExpandoContainer_t<TaskState_t>& out_state) = 0;
	unsigned int GetTag() const { return _tag; }
};

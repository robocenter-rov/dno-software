#pragma once

#include "PeripheryManager.h"
#include "ResourceLocker.h"
#include "SelfExpandoContainer.h"
#include "TaskState.h"

class Task_t {
protected:
	unsigned int _id;
public:
	Task_t(unsigned int id) : _id(id) {}
	virtual ~Task_t() {}
	virtual bool LockNeededResources(RESOURCE& out_locked_resource, int worker_id) { return true; }
	virtual bool UpdateState(SelfExpandoContainer_t<TaskState_t>& out_state) = 0;
	unsigned int GetId() const { return _id; }
};

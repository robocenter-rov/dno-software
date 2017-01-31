#pragma once

#include "PeripheryManager.h"
#include "ResourceLocker.h"
#include "SelfExpandoContainer.h"
#include "TaskState.h"

class Task_t {
protected:
	unsigned int _id;
	unsigned int _worker_id;
public:
	Task_t(unsigned int id) : _id(id), _worker_id(0) {}
	virtual ~Task_t() {}
	virtual bool LockNeededResources(RESOURCE& out_locked_resource) { return true; }
	virtual bool UpdateState(SelfExpandoContainer_t<TaskState_t>& out_state) = 0;
	unsigned int GetId() const { return _id; }
	void SetWorkerId(unsigned int worker_id);
};

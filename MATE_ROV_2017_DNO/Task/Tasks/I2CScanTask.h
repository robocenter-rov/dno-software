#pragma once

#include "../../Task/Task.h"

class I2CScanTask_t : public Task_t {
private:
	char _adresses[20];
	int _device_count;
	int _current_adress;
public:
	I2CScanTask_t(int tag);
	bool LockNeededResources(RESOURCE& out_locked_resource, int worker_id) override;
	bool UpdateState(SelfExpandoContainer_t<TaskState_t>& out_state) override;
};

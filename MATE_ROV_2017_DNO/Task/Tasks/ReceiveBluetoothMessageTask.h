#pragma once
#include "../../ResourceLocker/ResourceLocker.h"
#include "../../Task/Task.h"

class ReceiveBluetoothMessageTask_t : public Task_t {
private:
	char _message_buffer[7];
	ResourceLocker::AutoLocker_t _locker;
	int _bytes_readed;
public:
	ReceiveBluetoothMessageTask_t(unsigned int tag);

	bool LockNeededResources(RESOURCE& out_locked_resource, int worker_id) override;
	bool UpdateState(SelfExpandoContainer_t<TaskState_t>& out_state) override;
};

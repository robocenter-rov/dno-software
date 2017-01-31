#pragma once
#include "ResourceLocker.h"

class ReceiveBluetoothMessageTask_t : public Task_t {
private:
	char _message_buffer[7];
	ResourceLocker::AutoLocker_t _locker;
	int _bytes_readed;
public:
	ReceiveBluetoothMessageTask_t(unsigned int tag);

	bool LockNeededResources(RESOURCE& out_locked_resource) override;
	bool UpdateState(SelfExpandoContainer_t<TaskState_t>& out_state) override;
};

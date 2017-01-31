#pragma once

#include "ByteArray.h"
#include "ResourceLocker.h"

enum TASK_STATUS {
	TS_OK,
	TS_CANCELLED,

	TS_BLINKING,
	TS_SENDING,

	TS_BLUETOOTH_WAITING_FOR_CONNECTION,
	TS_BLUETOOTH_DATA_READED
};

struct TaskState_t {
	virtual ~TaskState_t() {}
	unsigned int task_id;
	unsigned int worker_id;
	TASK_STATUS status;
	TaskState_t(int task_id, int worker_id, TASK_STATUS status) : task_id(task_id), worker_id(worker_id), status(status) {}
	virtual ByteArray_t ToByteArray() const {
#pragma pack(push, 1)
		struct {
			unsigned int task_id;
			unsigned int worker_id;
			TASK_STATUS status;
		} buffer;
#pragma pack(pop)
		buffer.task_id = task_id;
		buffer.worker_id = worker_id;
		buffer.status = status;

		return ByteArray_t(buffer);
	}

	void *operator new(unsigned int size, void *ptr) {
		return ptr;
	}
};

struct OkTaskState_t : public TaskState_t {
	OkTaskState_t(unsigned int task_id, unsigned int worker_id) : TaskState_t(task_id, worker_id, TS_OK) {}
};

struct CancelledTaskState_t : public TaskState_t {
	CancelledTaskState_t(unsigned int task_id, unsigned int worker_id) : TaskState_t(task_id, worker_id, TS_CANCELLED) {}
};

struct BlinkFlashlightTaskState_t : public TaskState_t {
	unsigned int blinked_count;
	BlinkFlashlightTaskState_t(unsigned int task_id, unsigned int worker_id, unsigned int blinked_count)
		: TaskState_t(task_id, worker_id, TS_BLINKING), blinked_count(blinked_count) {}
	ByteArray_t ToByteArray() const override {
#pragma pack(push, 1)
		struct {
			unsigned int task_id;
			unsigned int worker_id;
			TASK_STATUS status;
			unsigned int blinked_count;
		} buffer;
#pragma pack(pop)

		buffer.task_id = task_id;
		buffer.worker_id = worker_id;
		buffer.status = status;
		buffer.blinked_count = blinked_count;

		return ByteArray_t(buffer);
	}
};

struct SendSensorDataTaskState_t : public TaskState_t {
	SendSensorDataTaskState_t(unsigned int task_id, unsigned int worker_id) : TaskState_t(task_id, worker_id, TS_BLINKING) {}
};

struct BluetoothWaitingForConnectionTaskState_t : public TaskState_t {
	BluetoothWaitingForConnectionTaskState_t(unsigned int task_id, unsigned int worker_id) : TaskState_t(task_id, worker_id, TS_BLUETOOTH_WAITING_FOR_CONNECTION) {}
};

struct BluetoothDataReadedTaskState_t : public TaskState_t {
	char message[7];
	BluetoothDataReadedTaskState_t(unsigned int task_id, unsigned int worker_id, char* msg) : TaskState_t(task_id, worker_id, TS_BLUETOOTH_DATA_READED) {
		memcpy(message, msg, 7);
	}
	ByteArray_t ToByteArray() const override {
#pragma pack(push, 1)
		struct {
			unsigned int task_id;
			unsigned int worker_id;
			TASK_STATUS status;
			char message[7];
		} buffer;
#pragma pack(pop)

		buffer.task_id = task_id;
		buffer.worker_id = worker_id;
		buffer.status = status;
		memcpy(buffer.message, message, 7);

		return ByteArray_t(buffer);
	}
};

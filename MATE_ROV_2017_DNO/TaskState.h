#pragma once

#include "ByteArray.h"
#include "ResourceLocker.h"

enum TASK_STATUS {
	TS_OK,
	TS_CANCELLED,

	TS_BLINKING,
	TS_SENDING,

	TS_BLUETOOTH_WAITING_FOR_CONNECTION,

	TS_I2C_SCANNING,
};

struct TaskState_t {
	virtual ~TaskState_t() {}
	unsigned int task_tag;
	TASK_STATUS status;
	TaskState_t(int task_tag, TASK_STATUS status) : task_tag(task_tag), status(status) {}
	virtual ByteArray_t ToByteArray() const {
#pragma pack(push, 1)
		struct {
			unsigned int task_tag;
			TASK_STATUS status;
		} buffer;
#pragma pack(pop)
		buffer.task_tag = task_tag;
		buffer.status = status;

		return ByteArray_t(buffer);
	}

	void *operator new(unsigned int size, void *ptr) {
		return ptr;
	}
};

struct OkTaskState_t : public TaskState_t {
	OkTaskState_t(unsigned int task_tag) : TaskState_t(task_tag, TS_OK) {}
};

struct CancelledTaskState_t : public TaskState_t {
	CancelledTaskState_t(unsigned int task_tag) : TaskState_t(task_tag, TS_CANCELLED) {}
};

struct BlinkFlashlightTaskState_t : public TaskState_t {
	unsigned int blinked_count;
	BlinkFlashlightTaskState_t(unsigned int task_tag, unsigned int blinked_count)
		: TaskState_t(task_tag, TS_BLINKING), blinked_count(blinked_count) {}
	ByteArray_t ToByteArray() const override {
#pragma pack(push, 1)
		struct {
			unsigned int task_tag;
			TASK_STATUS status;
			unsigned int blinked_count;
		} buffer;
#pragma pack(pop)

		buffer.task_tag = task_tag;
		buffer.status = status;
		buffer.blinked_count = blinked_count;

		return ByteArray_t(buffer);
	}
};

struct SendSensorDataTaskState_t : public TaskState_t {
	SendSensorDataTaskState_t(unsigned int task_tag) : TaskState_t(task_tag, TS_BLINKING) {}
};

struct BluetoothWaitingForConnectionTaskState_t : public TaskState_t {
	BluetoothWaitingForConnectionTaskState_t(unsigned int task_tag) : TaskState_t(task_tag, TS_BLUETOOTH_WAITING_FOR_CONNECTION) {}
};

struct BluetoothDataReadedTaskState_t : public TaskState_t {
	char message[7];

	BluetoothDataReadedTaskState_t(unsigned int task_tag, char* msg) : TaskState_t(task_tag, TS_OK) {
		if (msg) {
			memcpy(message, msg, 7);
		}
	}

	ByteArray_t ToByteArray() const override {
#pragma pack(push, 1)
		struct {
			unsigned int task_tag;
			TASK_STATUS status;
			char message[7];
		} buffer;
#pragma pack(pop)

		buffer.task_tag = task_tag;
		buffer.status = status;
		memcpy(buffer.message, message, 7);

		return ByteArray_t(buffer);
	}
};

struct I2CScanningTaskState_t : public TaskState_t {
	I2CScanningTaskState_t(unsigned int task_tag) : TaskState_t(task_tag, TS_I2C_SCANNING) {}
};

struct I2CScanningDoneTaskState_t : public TaskState_t {
	char adresses[20];
	int device_count;

	I2CScanningDoneTaskState_t(unsigned int task_tag, char* adrss, int count) : TaskState_t(task_tag, TS_OK) {
		memcpy(adresses, adrss, count);
		device_count = count;
	}

	ByteArray_t ToByteArray() const override {
		ByteArray_t res = ByteArray_t(6 + device_count);

		*((int*)(res.Get())) = task_tag;
		*((int*)(res.Get() + sizeof(int))) = status;
		*((int*)(res.Get() + sizeof(int) * 2)) = device_count;
		memcpy(res.Get() + 6, adresses, device_count);

		return res;
	}
};

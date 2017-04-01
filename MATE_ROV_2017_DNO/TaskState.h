#pragma once

#include "ByteArray.h"
#include "ResourceLocker.h"
#include "TaskId.h"

enum TASK_STATUS {
	TS_OK,
	TS_CANCELLED,

	TS_BLINKING,

	TS_BLUETOOTH_WAITING_FOR_CONNECTION,

	TS_I2C_SCANNING,
};

struct TaskState_t {
	virtual ~TaskState_t() {}
	TASK_ID task_id;
	unsigned int task_tag;
	TASK_STATUS status;
	TaskState_t(TASK_ID task_id, unsigned int task_tag, TASK_STATUS status) : task_id(task_id), task_tag(task_tag), status(status) {}
	virtual ByteArray_t GetAdditionalData() const {
		return ByteArray_t();
	}

	void *operator new(unsigned int size, void *ptr) {
		return ptr;
	}
};

struct OkTaskState_t : public TaskState_t {
	OkTaskState_t(TASK_ID task_id, unsigned int task_tag) : TaskState_t(task_id, task_tag, TS_OK) {}
};

struct CancelledTaskState_t : public TaskState_t {
	CancelledTaskState_t(TASK_ID task_id, unsigned int task_tag) : TaskState_t(task_id, task_tag, TS_CANCELLED) {}
};

struct BlinkFlashlightTaskState_t : public TaskState_t {
	unsigned int blinked_count;
	BlinkFlashlightTaskState_t(unsigned int task_tag, unsigned int blinked_count)
		: TaskState_t(TI_BLINK_FLASHLIGHT, task_tag, TS_BLINKING), blinked_count(blinked_count) {}
	ByteArray_t GetAdditionalData() const override {
		return ByteArray_t(blinked_count);
	}
};

struct SendSensorDataTaskState_t : public TaskState_t {
	SendSensorDataTaskState_t(unsigned int task_tag) : TaskState_t(TI_SEND_SENSOR_DATA, task_tag, TS_BLINKING) {}
};

struct BluetoothWaitingForConnectionTaskState_t : public TaskState_t {
	BluetoothWaitingForConnectionTaskState_t(unsigned int task_tag) : 
		TaskState_t(TI_RECEIVE_BLUETOOTH_MESSAGE, task_tag, TS_BLUETOOTH_WAITING_FOR_CONNECTION) {}
};

struct BluetoothDataReadedTaskState_t : public TaskState_t {
	char message[7];

	BluetoothDataReadedTaskState_t(unsigned int task_tag, char* msg) : TaskState_t(TI_RECEIVE_BLUETOOTH_MESSAGE, task_tag, TS_OK) {
		if (msg) {
			memcpy(message, msg, 7);
		}
	}

	ByteArray_t GetAdditionalData() const override {
		return ByteArray_t(message, 7);
	}
};

struct I2CScanningTaskState_t : public TaskState_t {
	I2CScanningTaskState_t(unsigned int task_tag) : 
		TaskState_t(TI_I2C_SCAN, task_tag, TS_I2C_SCANNING) {}
};

struct I2CScanningDoneTaskState_t : public TaskState_t {
	char adresses[20];
	int device_count;

	I2CScanningDoneTaskState_t(unsigned int task_tag, char* adrss, int count) : 
		TaskState_t(TI_I2C_SCAN, task_tag, TS_OK) {
		memcpy(adresses, adrss, count);
		device_count = count;
	}

	ByteArray_t GetAdditionalData() const override {
		ByteArray_t res = ByteArray_t(sizeof(int) + sizeof(int) * device_count);

		*reinterpret_cast<int*>(res.Get()) = device_count;
		memcpy(res.Get() + sizeof(int), adresses, device_count);

		return res;
	}
};

#pragma once

#include "ByteArray.h"
#include "ResourceLocker.h"

enum TASK_STATUS {
	TS_OK,
	TS_CANCELLED,

	TS_BLINKING,
};

struct TaskState_t {
	virtual ~TaskState_t() {}
	unsigned int task_id;
	unsigned int worker_id;
	TASK_STATUS status;
	TaskState_t(int task_id, int worker_id, TASK_STATUS status) : task_id(task_id), worker_id(worker_id), status(status) {}
	virtual ByteArray_t ToByteArray() const {
		struct {
			unsigned int task_id;
			unsigned int worker_id;
			TASK_STATUS status;
		} buffer;

		buffer.task_id = task_id;
		buffer.worker_id = worker_id;
		buffer.status = status;

		return ByteArray_t(&buffer, sizeof buffer);
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
		struct {
			unsigned int task_id;
			unsigned int worker_id;
			TASK_STATUS status;
			unsigned int blinked_count;
		} buffer;

		buffer.task_id = task_id;
		buffer.worker_id = worker_id;
		buffer.status = status;
		buffer.blinked_count = blinked_count;

		return ByteArray_t(&buffer, sizeof buffer);
	}
};

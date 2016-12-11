#pragma once

enum TASK_STATUS {
	TS_OK,
	TS_CANCELLED,
	TS_RESOURCE_LOCKED,

	TS_BLINKING,
};

struct TaskState_t {
	unsigned int task_id;
	unsigned int worker_id;
	TASK_STATUS status;
	TaskState_t() {}
	TaskState_t(int task_id, int worker_id, TASK_STATUS status) : task_id(task_id), worker_id(worker_id), status(status) {}
};

struct OkTaskState_t : public TaskState_t {
	OkTaskState_t() {}
	OkTaskState_t(unsigned int task_id, unsigned int worker_id) : TaskState_t(task_id, worker_id, TS_OK) {}
};

struct CancelledTaskState_t : public TaskState_t {
	CancelledTaskState_t() {}
	CancelledTaskState_t(unsigned int task_id, unsigned int worker_id) : TaskState_t(task_id, worker_id, TS_CANCELLED) {}
};

struct ResourceLockedTaskState_t : public TaskState_t {
	RESOURCE resource;
	unsigned int owner;
	ResourceLockedTaskState_t() {}
	ResourceLockedTaskState_t(unsigned int task_id, unsigned int worker_id, RESOURCE resource, int owner)
		: TaskState_t(task_id, worker_id, TS_RESOURCE_LOCKED), resource(resource), owner(owner) {}
};

struct BlinkFlashlightTaskState_t : public TaskState_t {
	unsigned int blinked_count;
	BlinkFlashlightTaskState_t() {}
	BlinkFlashlightTaskState_t(unsigned int task_id, unsigned int worker_id, unsigned int blinked_count)
		: TaskState_t(task_id, worker_id, TS_BLINKING), blinked_count(blinked_count) {}
};

union TaskStateUnion_t {
	TaskState_t base_state;
	OkTaskState_t ok_task_state;
	CancelledTaskState_t cancelled_task_state;
	ResourceLockedTaskState_t resource_locked_task_state;
	BlinkFlashlightTaskState_t blink_flashlight_task_state;

	TaskStateUnion_t() {}
};

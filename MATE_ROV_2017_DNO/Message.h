#pragma once

#include "Task.h"

enum MESSAGE_TYPE {
	MT_SET_POSITION,

	MT_FREE_WORKER,

	MT_SET_FLASHLIGHT_STATE,
	MT_BLINK_FLASHLIGHT,

	MT_TASK_STATE,

	MT_MAX_MESSAGE_TYPE_ID
};

#pragma pack(push, 1)

struct Message_t {
	MESSAGE_TYPE message_type;
	Message_t() {}
	Message_t(MESSAGE_TYPE message_type) : message_type(message_type) {}
};

struct SetPositionMessage_t : public Message_t {
	float position;
};

struct TaskMessage_t : public Message_t {
	int task_id;
};

struct FreeWorkerMessage_t : public Message_t {
	int worker_id;
};

struct SetFlashlightStateMessage_t : public TaskMessage_t {
	char state;
};

struct BlinkFlashlightMessage_t : public TaskMessage_t {
	int count;
};

struct TaskStateMessage_t : public Message_t {
	TaskStateUnion_t task_state;
	TaskStateMessage_t() {}
	TaskStateMessage_t(TaskStateUnion_t task_state) : Message_t(MT_TASK_STATE), task_state(task_state) {}
};

union MessageUnion_t {
	Message_t base_message;
	SetPositionMessage_t set_position_message;
	FreeWorkerMessage_t free_worker_message;
	SetFlashlightStateMessage_t set_flashlight_state_message;
	BlinkFlashlightMessage_t blink_flashlight_message;
	TaskStateMessage_t task_state_message;
	MessageUnion_t() {}
};

const unsigned int MESSAGE_SIZES[] = {
	sizeof(SetPositionMessage_t),
	sizeof(FreeWorkerMessage_t),
	sizeof(SetFlashlightStateMessage_t),
	sizeof(BlinkFlashlightMessage_t),
	sizeof(TaskStateMessage_t),
};

#pragma pack(pop)
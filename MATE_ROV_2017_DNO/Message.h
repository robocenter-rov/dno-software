#pragma once

enum MESSAGE_TYPE {
	MT_SET_POSITION,

	MT_SET_FLASHLIGHT_STATE,

	MT_MAX_MESSAGE_TYPE_ID
};

#pragma pack(push, 1)

struct Message_t {
	MESSAGE_TYPE message_type;
};

struct SetPositionMessage_t : public Message_t {
	float position;
};

struct SetFlashlightStateMessage_t : public Message_t {
	char state;
};

#pragma pack(pop)
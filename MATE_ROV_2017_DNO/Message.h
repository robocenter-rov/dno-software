#pragma once

enum MESSAGE_TYPE {
	SET_POSITION_MT,

	MAX_MESSAGE_TYPE_ID
};

struct Message_t {
	MESSAGE_TYPE message_type;
};

struct SetPositionMessage_t : public Message_t {
	float position;
};
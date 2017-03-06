#include "Communicator.h"

#define reg_msg_info(id, name) \
	void Communicator_t::SetOn##name##Receive(name##ReceiveFunction_t callback, void* data) {\
		_On##name##Receive.data = data;\
		_On##name##Receive.callback = callback;\
	}
#define reg_msg_begin_fields_list
#define reg_msg_field(type, name)
#define reg_msg_last_field(type, name)
#define reg_msg_end_fields_list
#include "../../Messages.h"
#undef reg_msg_info
#undef reg_msg_begin_fields_list
#undef reg_msg_field
#undef reg_msg_last_field
#undef reg_msg_end_fields_list

Communicator_t::Communicator_t() {
#define reg_msg_info(id, name) SetOn##name##Receive(nullptr, nullptr);
#define reg_msg_begin_fields_list
#define reg_msg_field(type, name)
#define reg_msg_last_field(type, name)
#define reg_msg_end_fields_list
#include "../../Messages.h"
#undef reg_msg_info
#undef reg_msg_begin_fields_list
#undef reg_msg_field
#undef reg_msg_last_field
#undef reg_msg_end_fields_list
}

#pragma once
#include "TaskState.h"
#include "Exceptions.h"
#include "SensorData.h"

class Communicator_t {
protected:

#define reg_msg_info(id, name) typedef int(*name##ReceiveFunction_t)
#define reg_msg_begin_fields_list (void* data,
#define reg_msg_field(type, name) type name,
#define reg_msg_last_field(type, name) type name
#define reg_msg_end_fields_list );
#include "Messages.h"
#undef reg_msg_info
#undef reg_msg_begin_fields_list
#undef reg_msg_field
#undef reg_msg_last_field
#undef reg_msg_end_fields_list

#define reg_msg_info(id, name) struct On##name##ReceiveCallback_t {void* data; name##ReceiveFunction_t callback;} _On##name##Receive;
#define reg_msg_begin_fields_list
#define reg_msg_field(type, name)
#define reg_msg_last_field(type, name)
#define reg_msg_end_fields_list
#include "Messages.h"
#undef reg_msg_info
#undef reg_msg_begin_fields_list
#undef reg_msg_field
#undef reg_msg_last_field
#undef reg_msg_end_fields_list

public:
	Communicator_t();
	virtual ~Communicator_t() {}

#define reg_msg_info(id, name) void SetOn##name##Receive(int(*)
#define reg_msg_begin_fields_list (void* data,
#define reg_msg_field(type, name) type name,
#define reg_msg_last_field(type, name) type name),
#define reg_msg_end_fields_list void* data = nullptr);
#include "Messages.h"
#undef reg_msg_info
#undef reg_msg_begin_fields_list
#undef reg_msg_field
#undef reg_msg_last_field
#undef reg_msg_end_fields_list

	virtual int SendException(const Exceptions::Exception_t* exception) = 0;
	virtual int SendTaskState(const TaskState_t* task_state) = 0;
	virtual int SendSensorData(const SensorData_t* sensor_data) = 0;

	virtual int Begin() = 0;
	virtual int Connected() = 0;
	virtual int Update() = 0;
};

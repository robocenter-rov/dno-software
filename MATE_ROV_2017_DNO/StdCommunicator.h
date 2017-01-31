#pragma once

#include "ConnectionProvider.h"
#include "Communicator.h"
#include "Map.h"

class StdCommunicator_t : public Communicator_t {
private:
	typedef int(*ReceiveMessageFunction_t)(StdCommunicator_t* _this, void* data, unsigned int size);
#define reg_msg_info(id, name) static int Receive##name##Message(StdCommunicator_t* _this, void* data, unsigned int size);
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

	ConnectionProvider_t* _connection_provider;

	Map_t<unsigned int, ReceiveMessageFunction_t> _message_receivers;
public:
	StdCommunicator_t(ConnectionProvider_t* connection_provider);

	int SendException(const Exceptions::Exception_t* exception) override;
	int SendTaskState(const TaskState_t* task_state) override;
	int SendSensorData(const SensorData_t* sensor_data) override;

	int Update() override;
	int Begin() override;
	int Connected() override;
};

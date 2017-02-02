#include "StdCommunicator.h"
#include "Exceptions.h"
#include "MathConstants.h"

#pragma pack(push, 1)

struct Message_t {
	byte id;
};
#define reg_msg_info(id, name) struct name##Message_t : Message_t
#define reg_msg_begin_fields_list {
#define reg_msg_field(type, name) type name;
#define reg_msg_last_field(type, name) type name;
#define reg_msg_end_fields_list };
#include "Messages.h"
#undef reg_msg_info
#undef reg_msg_begin_fields_list
#undef reg_msg_field
#undef reg_msg_last_field
#undef reg_msg_end_fields_list

#pragma pack(pop)

StdCommunicator_t::StdCommunicator_t(ConnectionProvider_t* connection_provider) : _connection_provider(connection_provider) {
	int msg_receivers_count = 0;

#define reg_msg_info(id, name) msg_receivers_count++;
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

	_message_receivers.Reserve(msg_receivers_count);

#define reg_msg_info(id, name) _message_receivers.Add(id, Receive##name##Message);
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
}

#ifdef _DEBUG
#define reg_msg_info(id, name) static int StdCommunicator_t::Receive##name##Message(StdCommunicator_t* _this, void* data, unsigned int size) { \
	if (sizeof(name##Message_t) != size) { \
		ThrowException(Exceptions::EC_SC_MESSAGE_WRONG_SIZE); \
		return 1; \
	} \
	auto msg = reinterpret_cast<name##Message_t*>(data); \
	Serial.println("StdCommunicator: receive message "#name); \
	if (_this->_On##name##Receive.callback == nullptr) { \
		ThrowException(Exceptions::EC_SC_RECEIVER_NOT_SETTED); \
		return 1; \
	} \
	return _this->_On##name##Receive.callback(_this->_On##name##Receive.data,
#else
#define reg_msg_info(id, name) static int StdCommunicator_t::Receive##name##Message(StdCommunicator_t* _this, void* data, unsigned int size) { \
	if (sizeof(name##Message_t) != size) { \
		ThrowException(Exceptions::EC_SC_MESSAGE_WRONG_SIZE); \
		return 1; \
	} \
	if (_this->_On##name##Receive.callback == nullptr) { \
		ThrowException(Exceptions::EC_SC_RECEIVER_NOT_SETTED); \
		return 1; \
	} \
	auto msg = reinterpret_cast<name##Message_t*>(data); \
	_this->_On##name##Receive.callback(_this->_On##name##Receive.data,
#endif
#define reg_msg_begin_fields_list
#define reg_msg_field(type, name) msg->name,
#define reg_msg_last_field(type, name) msg->name
#define reg_msg_end_fields_list ); }
#include "Messages.h"
#undef reg_msg_info
#undef reg_msg_begin_fields_list
#undef reg_msg_field
#undef reg_msg_last_field
#undef reg_msg_end_fields_list

int StdCommunicator_t::SendException(const Exceptions::Exception_t* exception) {
	if (!Connected()) {
		ThrowException(Exceptions::EC_SC_NOT_CONNECTED);
		return 1;
	}

#ifdef _DEBUG

	Serial.print("Sending exception: ");
	Serial.println(exception->GetCode());

#endif

	_connection_provider->BeginPacket();
	_connection_provider->Write(static_cast<char>(100));
	_connection_provider->Write(exception->GetCode());
	_connection_provider->EndPacket();
	return 0;
}

int StdCommunicator_t::SendTaskState(const TaskState_t* task_state) {
	if (!Connected()) {
		ThrowException(Exceptions::EC_SC_NOT_CONNECTED);
		return 1;
	}

	if (!task_state) {
		ThrowException(Exceptions::EC_BAD_ARGS);
		return 1;
	}

#ifdef _DEBUG

	Serial.print("Sending task state: ");
	Serial.println(task_state->status);

#endif

	auto task_state_bytes = task_state->ToByteArray();
	_connection_provider->BeginPacket();
	_connection_provider->Write(1);
	_connection_provider->Write(task_state_bytes.Get(), task_state_bytes.GetSize());
	_connection_provider->EndPacket();
	return 0;
}

int StdCommunicator_t::SendSensorData(const SensorData_t* sensor_data) {
	if (!Connected()) {
		ThrowException(Exceptions::EC_SC_NOT_CONNECTED);
		return 1;
	}

#ifdef _DEBUG

	Serial.println("Sending sensor data: ");

	Serial.print("Rotation X");
	Serial.println(sensor_data->rotationData.RotationX);
	Serial.print("Rotation Y");
	Serial.println(sensor_data->rotationData.RotationY);
	Serial.print("Rotation Z");
	Serial.println(sensor_data->rotationData.RotationZ);

#endif

#pragma pack(push, 1)
	struct {
		int Rx;
		int Ry;
		int Rz;
		int depth;
	} sendingData{
		sensor_data->rotationData.RotationX / PI2 * (1 << 15),
		sensor_data->rotationData.RotationY / PI2 * (1 << 15),
		sensor_data->rotationData.RotationZ / PI2 * (1 << 15),
		sensor_data->depthData
	};
#pragma pack(pop)

	_connection_provider->BeginPacket();
	_connection_provider->Write(2);
	_connection_provider->Write(&sendingData, sizeof(sendingData));
	_connection_provider->EndPacket();
	return 0;
}

int StdCommunicator_t::Update() {
	unsigned int readed_bytes = 0;

	while(true) {
		if (_connection_provider->Receive(readed_bytes)) {
			return 1;
		}

		if (!readed_bytes) {
			return 0;
		}

		unsigned int message_type_id = reinterpret_cast<Message_t*>(_connection_provider->Buffer())->id;

		ReceiveMessageFunction_t receiver;
		if (!_message_receivers.Find(message_type_id, receiver)) {
			ThrowException(Exceptions::EC_SC_MESSAGE_WRONG_TYPE_ID);
			return 1;
		}

		if (receiver(this, reinterpret_cast<char*>(_connection_provider->Buffer()), readed_bytes)) {
			if (Exceptions::GetLastCode() != Exceptions::EC_SC_MESSAGE_WRONG_SIZE) {
				return 1;
			}
			Exceptions::Release();
		}
	}
}

int StdCommunicator_t::Begin() {
	return _connection_provider->Begin();
}

int StdCommunicator_t::Connected() {
	return _connection_provider->Connected();
}

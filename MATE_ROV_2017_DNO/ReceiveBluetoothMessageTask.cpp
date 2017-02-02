#include "ReceiveBluetoothMessageTask.h"


ReceiveBluetoothMessageTask_t::ReceiveBluetoothMessageTask_t(unsigned tag) : Task_t(tag), _locker(R_BLUETOOTH) {
	_bytes_readed = 0;
}

bool ReceiveBluetoothMessageTask_t::LockNeededResources(RESOURCE& out_locked_resource, int worker_id) {
	if (!_locker.Lock(worker_id)) {
		out_locked_resource = R_BLUETOOTH;
		return false;
	}
	return true;
}

bool ReceiveBluetoothMessageTask_t::UpdateState(SelfExpandoContainer_t<TaskState_t>& out_state) {
	if (Serial1.available()) {
		char byte;
		while (_bytes_readed < 7 && (byte = Serial1.read()) > 0) {

			LOG("Byte readed fron bluetooth:");
			LOGLN(byte);

			_message_buffer[_bytes_readed++] = byte;
		}

		if (_bytes_readed == 7) {
			out_state = BluetoothDataReadedTaskState_t(_tag, _message_buffer);
			return true;
		}
	}

	out_state = BluetoothWaitingForConnectionTaskState_t(_tag);

	return false;
}

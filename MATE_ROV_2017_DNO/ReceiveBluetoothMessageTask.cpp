#include "ReceiveBluetoothMessageTask.h"


ReceiveBluetoothMessageTask_t::ReceiveBluetoothMessageTask_t(unsigned tag) : Task_t(tag), _locker(R_BLUETOOTH) {
	_bytes_readed = 0;
}

bool ReceiveBluetoothMessageTask_t::LockNeededResources(RESOURCE& out_locked_resource) {
	if (!_locker.Lock(_worker_id)) {
		out_locked_resource = R_BLUETOOTH;
		return false;
	}
	return true;
}

bool ReceiveBluetoothMessageTask_t::UpdateState(SelfExpandoContainer_t<TaskState_t>& out_state) {
	if (Serial1.available()) {
		char byte;
		while (_bytes_readed < 7 && (byte = Serial.read()) > 0) {
			_message_buffer[_bytes_readed++] = byte;
		}

		if (_bytes_readed == 7) {
			out_state = BluetoothDataReadedTaskState_t(_id, _worker_id, _message_buffer);
			return true;
		} else {
			out_state = BluetoothWaitingForConnectionTaskState_t(_id, _worker_id);
		}
	}
	return false;
}

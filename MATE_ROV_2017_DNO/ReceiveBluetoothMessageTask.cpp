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
	bool cur_state = false;
	if (Serial2.available()) {
		char byte;
		while (Serial2.available() == 0)
		{
		}

		while ((byte = Serial2.read()) > 0) {
			if (cur_state == false) {
				if (byte == 0x0A) {
					cur_state = true;
				}
			}
			else {
				if (_bytes_readed < 7) {
					LOG("Byte readed fron bluetooth:");
					LOGLN(byte);
					_message_buffer[_bytes_readed++] = byte;
				}
			}
		}

		if (_bytes_readed == 7) {
			out_state = BluetoothDataReadedTaskState_t(_tag, _message_buffer);
			return true;
		}
	}

	out_state = BluetoothWaitingForConnectionTaskState_t(_tag);

	return false;
}

bool ReceiveBluetoothMessageTask_t::Awake() {
	/*while (Serial2.available()) {
		Serial2.read();
	}*/
}

bool ReceiveBluetoothMessageTask_t::Destroy() {

}

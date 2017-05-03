#include "BluetoothPeriphery.h"



BluetoothPeriphery_t::BluetoothPeriphery_t(Stream* serial)
{
	_serial = serial;
	_bytes_readed = 0;
}


BluetoothPeriphery_t::~BluetoothPeriphery_t() {}

bool BluetoothPeriphery_t::ReceiveBluetoothMessage(char* buff)
{
	if (_serial->available()) {
		char byte;
		while (_bytes_readed < 7 && (byte = Serial2.read()) > 0) {

			LOG("Byte readed from bluetooth:");
			LOGLN(byte);
			_message_buffer[_bytes_readed++] = byte;
			//buff[_bytes_readed++] = byte;
		}
		if (_bytes_readed == 7)
		{
			memcpy(buff, _message_buffer, sizeof(char) * 7);
			//buff = _message_buffer;
			return true;
		}
	}
	return false;

}

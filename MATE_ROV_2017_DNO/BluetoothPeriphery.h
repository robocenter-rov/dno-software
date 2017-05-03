#pragma once

#include "Arduino.h"
#include "Debug.h"

class BluetoothPeriphery_t
{
private:
	Stream* _serial;
	char _message_buffer[7];
	int _bytes_readed;

public:
	BluetoothPeriphery_t(Stream* serial);
	~BluetoothPeriphery_t();
	bool ReceiveBluetoothMessage(char* buff);
};


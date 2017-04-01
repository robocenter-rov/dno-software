#include "UARTConnectionProvider.h"
#include "Debug.h"

//uint32_t UARTConnectionProvider::HashLy(const char* str) {
//	uint32_t hash = 0;
//	for (; *str; str++)
//		hash = (hash * 1664525) + static_cast<unsigned char>(*str) + 1013904223;
//	return hash;
//}

#define END             0300    /* indicates end of packet */
#define ESC             0333    /* indicates byte stuffing */
#define ESC_END         0334    /* ESC ESC_END means END data byte */
#define ESC_ESC         0335    /* ESC ESC_ESC means ESC data byte */

uint32_t HashLy(char byte, uint32_t hash) {
	return (hash * 1664525) + byte + 1013904223;
}

UARTConnectionProvider::UARTConnectionProvider(Stream* serial, unsigned int buffer_size) : ConnectionProvider_t(buffer_size)
{
	_serial = serial;
	_current_hash = 0;
	_received = 0;
	_in_esc = false;
}

UARTConnectionProvider::~UARTConnectionProvider()
{
}

int UARTConnectionProvider::Begin() {
	LOGLN("UART connection begin");

	return 0;
}

void UARTConnectionProvider::Reset() {
	_current_hash = 0;
	_received = 0;
}

int UARTConnectionProvider::Connected() {
	return 1;
}

int UARTConnectionProvider::BeginPacket() {
	_current_hash = 0;
	return 0;
}

int UARTConnectionProvider::Write(void* buffer, unsigned int size) {
	char* buff = static_cast<char*>(buffer);
	int receivebytes = 0;
	while (size--) {
		switch (*buff) {
			case END:
				_serial->write(ESC);
				_serial->write(ESC_END);
				break;
			case ESC:
				_serial->write(ESC);
				_serial->write(ESC_ESC);
				break;
			default:
				_serial->write(*buff);
				_current_hash = HashLy(*buff, _current_hash);
		}
		buff++;
	}
	return 0;
}

int UARTConnectionProvider::Write(char c) {
	_current_hash = HashLy(c, _current_hash);
	_serial->write(c);
	return 0;
}

int UARTConnectionProvider::Write(int i) {
	auto t = reinterpret_cast<char*>(&i);
	_serial->write(t, sizeof(int));
	return 0;
}

int UARTConnectionProvider::EndPacket() {
	uint32_t hash = _current_hash;
	UARTConnectionProvider::Write(&hash, sizeof(uint32_t));
	_serial->write(END);
	return 0;
}

int UARTConnectionProvider::Send(void* buffer, unsigned int size) {
	BeginPacket();
	Write(buffer, size);
	EndPacket();
	return  0;
}

int UARTConnectionProvider::Receive(unsigned int& readed_bytes) {
	int c;

	while ((c = _serial->read()) >= 0) {
		if (c == ESC) {
			_in_esc = true;
			continue;
		}

		if (_in_esc) {
			switch (c) {
			case ESC_END:
				c = END;
				break;
			case ESC_ESC:
				c = ESC;
				break;
			}
			_in_esc = false;
		} else if (c == END) {
			if (_received < 5) {
				_received = 0;
				return 0;
			}
			char* hash_ptr = _buffer + _received - sizeof(uint32_t);
			uint32_t msg_hash = 0;

			for (char *p = _buffer; p != hash_ptr; p++) {
				msg_hash = HashLy(*p, msg_hash);
			}

			if (msg_hash == *reinterpret_cast<uint32_t*>(hash_ptr)) {
				readed_bytes = _received - sizeof(uint32_t);
			}

			_received = 0;
			return 0;
		}

		if (_received < _buffer_size) {
			_buffer[_received++] = c;
		}
		else {
			_received = 0;
			ThrowException(Exceptions::EC_CP_MESSAGE_TOO_LARGE);
			return 1;
		}
	}

	return 0;
}

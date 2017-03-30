#include "UARTConnectionProvider.h"

//uint32_t UARTConnectionProvider::HashLy(const char* str) {
//	uint32_t hash = 0;
//	for (; *str; str++)
//		hash = (hash * 1664525) + static_cast<unsigned char>(*str) + 1013904223;
//	return hash;
//}

uint32_t UARTConnectionProvider::HashLy(char byte, uint32_t hash) {
	return (hash * 1664525) + byte + 1013904223;
}

UARTConnectionProvider::UARTConnectionProvider(Stream* serial, unsigned int buffer_size): ConnectionProvider_t(buffer_size)
{
	_serial = serial;
	_current_hash = 0;
}

UARTConnectionProvider::~UARTConnectionProvider()
{
}

int UARTConnectionProvider::Begin() {
#if _DEBUG
	Serial.println("UART connection begin");
#endif

	return 0;
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
				_msg += ESC + ESC_END;
				break;
			case ESC:
				_serial->write(ESC);
				_serial->write(ESC_ESC);
				_msg += ESC + ESC_ESC;
				break;
			default:
				_serial->write(*buff);
				_msg += *buff;
		}
		buff++;
	}
	Serial.write(HashFAQ6(_msg));
	return 0;
}

int UARTConnectionProvider::Write(char c) {

	Serial.write(c);
	_serial->write(c);
	return 0;
}

int UARTConnectionProvider::Write(int i) {

	auto t = reinterpret_cast<char*>(&i);
	_serial->write(t, sizeof(int));
	return 0;
}

int UARTConnectionProvider::Write(unsigned int i) {

	return Write(static_cast<int>(i));
}

int UARTConnectionProvider::EndPacket() {

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

	char c;
	int received = 0;

	while (true) {

		c = Serial.read();

		switch (c) {
		case END:
			if (received) return received;
			else break;
		case ESC:
			c = Serial.read();
			switch (c) {
			case ESC_END:
				c = END;
				break;
			case ESC_ESC:
				c = ESC;
				break;
			}
			break;
		default:
			if (received < readed_bytes) {
				_buffer[received++] = c;
			} else {
				char* buff = _buffer + received * sizeof(uint8_t) - sizeof(uint32_t);
				uint32_t* buffHash = reinterpret_cast<uint32_t*>(buff);
				uint32_t receivedMsg = HashFAQ6(_buffer);

				if (receivedMsg == *buffHash && received <= readed_bytes) {
					return 0;
				} else {
					if (received > _buffer_size) {
						ThrowException(Exceptions::EC_CP_MESSAGE_TOO_LARGE);
						return 1;
					} else {
						received = 0;
						return  1;
					}
				}
			}
			break;
		}
	}
}

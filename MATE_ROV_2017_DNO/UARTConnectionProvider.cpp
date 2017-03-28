#include "UARTConnectionProvider.h"

uint32_t UARTConnectionProvider::HashFAQ6(const char* str) {

	uint32_t hash = 0;
	for (; *str; str++)
	{
		hash += static_cast<unsigned char>(*str);
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash;
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
	Serial.write(END);
	return 0;
}

int UARTConnectionProvider::Write(void* buffer, unsigned int size) {

	char* buff = static_cast<char*>(buffer);
	int receivebytes = 0;
	while (size--) {
		switch (*buff) {
			case END:
				Serial.write(ESC);
				Serial.write(ESC_END);
				_msg += ESC + ESC_END;
				break;
			case ESC:
				Serial.write(ESC);
				Serial.write(ESC_ESC);
				_msg += ESC + ESC_ESC;
				break;
			default:
				Serial.write(*buff);
				_msg += *buff;
		}
		buff++;
	}
	Serial.write(HashFAQ6(_msg));
	return 0;
}

int UARTConnectionProvider::Write(char c) {

	Serial.write(c);
	return 0;
}

int UARTConnectionProvider::Write(int i) {

	auto t = reinterpret_cast<char*>(&i);
	Serial.write(t, sizeof(int));
	return 0;
}

int UARTConnectionProvider::Write(unsigned int i) {

	return Write(static_cast<int>(i));
}

int UARTConnectionProvider::EndPacket() {

	Serial.write(END);
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

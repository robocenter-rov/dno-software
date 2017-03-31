#pragma once
#include "ConnectionProvider.h"
#include "Arduino.h"
#include "Exceptions.h"

class UARTConnectionProvider : public ConnectionProvider_t {
private:
	Stream* _serial;
	uint32_t _current_hash;

	unsigned int _received;

	bool _in_esc;
public:
	UARTConnectionProvider(Stream* serial, unsigned int buffer_size);
	~UARTConnectionProvider();
	int Begin() override;
	void Reset() override;
	int Connected() override;
	int BeginPacket() override;
	int Write(void* buffer, unsigned int size) override;
	int Write(char c) override;
	int Write(int i) override;
	//int Write(unsigned int i) override;
	int EndPacket() override;
	int Send(void* buffer, unsigned int size) override;
	int Receive(unsigned int& readed_bytes) override;
};


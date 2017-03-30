#pragma once
#include "ConnectionProvider.h"
#include "Arduino.h"
#include "Exceptions.h"

#define END             0300    /* indicates end of packet */
#define ESC             0333    /* indicates byte stuffing */
#define ESC_END         0334    /* ESC ESC_END means END data byte */
#define ESC_ESC         0335    /* ESC ESC_ESC means ESC data byte */

class UARTConnectionProvider : public ConnectionProvider_t {
private:
	const char* _msg = "";
	uint32_t HashLy(char byte, uint32_t hash);

public:
	UARTConnectionProvider();
	~UARTConnectionProvider();
	int Begin() override;
	void Reset() override;
	int Connected() override;
	int BeginPacket() override;
	int Write(void* buffer, unsigned int size) override;
	int Write(char c) override;
	int Write(int i) override;
	int Write(unsigned int i) override;
	int EndPacket() override;
	int Send(void* buffer, unsigned int size) override;
	int Receive(unsigned int& readed_bytes) override;
};


#pragma once
#include "ConnectionProvider.h"
#include <Ethernet.h>
#include "EthernetUdp.h"

class UdpConnectionProvider_t : public ConnectionProvider_t {
	byte _mac[6];
	IPAddress _local_ip;
	unsigned int _local_port;

	IPAddress _remote_ip;
	unsigned int _remote_port;

	EthernetUDP _udp;

	unsigned long _time_out = 0;
	unsigned long _time_of_last_received_message;
public:
	UdpConnectionProvider_t(unsigned int buffer_size, byte mac[6], IPAddress ip, unsigned int port, unsigned long time_out = 0);
	UdpConnectionProvider_t(unsigned int buffer_size, byte mac[6], unsigned int local_port, unsigned long time_out = 0);
	int Begin() override;
	void Reset() override;
	int Connected() override;
	int BeginPacket() override;
	int Write(void* buffer, unsigned int size) override;
	int Write(char c) override;
	int Write(int c) override;
	int Write(unsigned int c) override;
	int EndPacket() override;
	int Send(void* buffer, unsigned int size) override;
	int Receive(unsigned int& readed_bytes) override;
};

#pragma once
#include "Message.h"
#include <Ethernet.h>
#include <EthernetUdp.h>

#define MAX_MESSAGE_SIZE 100

class Communicator_t {
protected:
	char buffer[MAX_MESSAGE_SIZE];
public:
	virtual Message_t* ReceiveMessage();
	virtual void SendMessage(Message_t*, unsigned int message_size);
};

class UDPCommunicator_t : public Communicator_t {
private:
	byte _mac[6];
	IPAddress _local_ip;
	unsigned int _local_port;

	IPAddress _last_remote_ip;
	unsigned int _last_remote_port;

	EthernetUDP _udp;
public:
	UDPCommunicator_t(byte mac[6], IPAddress ip, unsigned int local_port);
	UDPCommunicator_t(byte mac[6], unsigned int local_port);
	Message_t* ReceiveMessage() override;
	void SendMessage(Message_t* message, unsigned int message_size) override;
};
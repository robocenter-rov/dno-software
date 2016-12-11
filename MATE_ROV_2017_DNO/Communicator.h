#pragma once
#include "Message.h"
#include <Ethernet.h>
#include <EthernetUdp.h>

#define MAX_MESSAGE_SIZE 100

class Communicator_t {
public:
	virtual ~Communicator_t() {}
	virtual bool ReceiveMessage(MessageUnion_t& out_message) { return false; };
	virtual void SendMessage(const Message_t* message) {};
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
	bool ReceiveMessage(MessageUnion_t& out_message) override;
	void SendMessage(const Message_t* message) override;
};

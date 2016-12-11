#include "Communicator.h"

bool Communicator_t::ReceiveMessage(Message_t* out_message) {
	return false;
}

void Communicator_t::SendMessage(Message_t* msg, unsigned int message_size) {}

UDPCommunicator_t::UDPCommunicator_t(byte mac[6], IPAddress ip, unsigned int local_port) {
	memcpy(mac, _mac, sizeof(byte) * 6);
	_local_ip = ip;
	_local_port = local_port;

	Ethernet.begin(_mac, _local_ip);
	_udp.begin(_local_port);
}

UDPCommunicator_t::UDPCommunicator_t(byte mac[6], unsigned int local_port) {
	memcpy(_mac, mac, sizeof(byte) * 6);
	_local_port = local_port;

	Serial.println("begin");

	Ethernet.begin(mac);

	Serial.print("My IP address: ");
	for (byte thisByte = 0; thisByte < 4; thisByte++) {
		// print the value of each byte of the IP address:
		Serial.print(Ethernet.localIP()[thisByte], DEC);
		Serial.print(".");
	}

	Serial.println();

	_local_ip = Ethernet.localIP();

	_udp.begin(_local_port);
}

bool UDPCommunicator_t::ReceiveMessage(Message_t* message) {
	if (int packet_size = _udp.parsePacket()) {
		char* buffer = reinterpret_cast<char*>(message);

		_last_remote_ip = _udp.remoteIP();
		_last_remote_port = _udp.remotePort();

		Serial.print("Message received, size: ");
		Serial.println(packet_size);
		_udp.read(buffer, UDP_TX_PACKET_MAX_SIZE);

		Serial.print("Message: ");

		for (int i = 0; i < packet_size; i++) {
			Serial.print(static_cast<int>(buffer[i]));
			Serial.print(' ');
		}

		Serial.print('\n');
		Serial.print("Message type: ");
		Serial.println(message->message_type);

		return true;
	}

	return false;
}

void UDPCommunicator_t::SendMessage(Message_t* message, unsigned int message_size) {
	_udp.beginPacket(_last_remote_ip, _last_remote_ip);
	_udp.write(reinterpret_cast<char*>(message), message_size);
	_udp.endPacket();
}

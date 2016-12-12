#include "Communicator.h"

UDPCommunicator_t::UDPCommunicator_t(byte mac[6], IPAddress ip, unsigned int local_port) {
	memcpy(mac, _mac, sizeof(byte) * 6);
	_local_ip = ip;
	_local_port = local_port;

	Ethernet.begin(_mac, _local_ip);

	_udp.begin(_local_port);

#ifdef _DEBUG
	Serial.print("My IP address: ");
	for (byte thisByte = 0; thisByte < 4; thisByte++) {
		// print the value of each byte of the IP address:
		Serial.print(Ethernet.localIP()[thisByte], DEC);
		Serial.print(".");
	}
#endif
}

UDPCommunicator_t::UDPCommunicator_t(byte mac[6], unsigned int local_port) {
	memcpy(_mac, mac, sizeof(byte) * 6);
	_local_port = local_port;

#if _DEBUG
	Serial.println("begin");
#endif

	Ethernet.begin(mac);

#if _DEBUG
	Serial.print("My IP address: ");
	for (byte thisByte = 0; thisByte < 4; thisByte++) {
		// print the value of each byte of the IP address:
		Serial.print(Ethernet.localIP()[thisByte], DEC);
		Serial.print(".");
	}

	Serial.println();

#endif

	_local_ip = Ethernet.localIP();

	_udp.begin(_local_port);
}

bool UDPCommunicator_t::ReceiveMessage(MessageUnion_t& message) {
	if (int packet_size = _udp.parsePacket()) {
#ifdef _DEBUG
		Serial.print("Message received, size: ");
		Serial.println(packet_size);
		Serial.print("Buffer size: ");
		Serial.println(sizeof(MessageUnion_t));
		delay(1000);
#endif _DEBUG

		if (packet_size > sizeof(MessageUnion_t)) {
#ifdef _DEBUG
			Serial.println("Message too big, flush packet");
#endif
			_udp.flush();
			return;
		}

		char* buffer = reinterpret_cast<char*>(&message);

		_last_remote_ip = _udp.remoteIP();
		_last_remote_port = _udp.remotePort();

		int readed_bytes = _udp.read(buffer, sizeof(MessageUnion_t));

#ifdef _DEBUG
		Serial.print("Bytes readed: ");
		Serial.println(readed_bytes);
		Serial.print("Message: ");

		for (int i = 0; i < packet_size; i++) {
			Serial.print(static_cast<int>(buffer[i]));
			Serial.print(' ');
		}

		Serial.print('\n');
		Serial.print("Message type: ");
		Serial.println(message.base_message.message_type);
		delay(200);
#endif

		return true;
	}

	return false;
}

void UDPCommunicator_t::SendMessage(const Message_t* message) {
#ifdef _DEBUG
	Serial.print("Sending message to: ");
	_last_remote_ip.printTo(Serial);
	Serial.print(":");
	Serial.println(_last_remote_port);
	Serial.print("Message type: ");
	Serial.println(message->message_type);
	Serial.print("Message size: ");
	Serial.println(MESSAGE_SIZES[message->message_type]);
	Serial.println("Message: ");
	for (int i = 0; i < MESSAGE_SIZES[message->message_type]; i++) {
		Serial.print(int(((char*)(message))[i]));
		Serial.print(' ');
	}
	Serial.println();
	delay(200);
#endif
	_udp.beginPacket(_last_remote_ip, _last_remote_ip);
	_udp.write(reinterpret_cast<const char*>(message), MESSAGE_SIZES[message->message_type]);
	_udp.endPacket();
}

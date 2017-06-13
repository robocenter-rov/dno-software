#include "Debug.h"
#include "UdpConnectionProvider.h"
#include "Exceptions.h"
#include <HardwareSerial.h>

UdpConnectionProvider_t::UdpConnectionProvider_t(
	unsigned int buffer_size, byte mac[6], IPAddress ip, unsigned int port, unsigned long time_out
	) : ConnectionProvider_t(buffer_size) 
{
	memcpy(mac, _mac, sizeof(byte) * 6);
	_local_ip = ip;
	_local_port = port;
	_remote_ip = IPAddress();
	_remote_port = 0;

	_time_out = time_out;
	_time_of_last_received_message = 0;
}

UdpConnectionProvider_t::UdpConnectionProvider_t(
	unsigned int buffer_size, byte mac[6], unsigned int local_port, unsigned long time_out
	) : ConnectionProvider_t(buffer_size) 
{
	memcpy(_mac, mac, sizeof(byte) * 6);
	_local_ip = IPAddress();
	_local_port = local_port;
	_remote_ip = IPAddress();
	_remote_port = 0;

	_time_out = time_out;
	_time_of_last_received_message = 0;
}

int UdpConnectionProvider_t::Begin() {
#if _DEBUG
	Serial.println("Udp connection begin");
#endif
	if (_local_ip) {
#if _DEBUG
		Serial.println("Using predefined ip: ");
#endif
		Ethernet.begin(_mac, _local_ip);
	} else {
#if _DEBUG
		Serial.println("Using DHCP");
#endif
		if (!Ethernet.begin(_mac)) {
			ThrowException(Exceptions::EC_CP_CANT_TAKE_ADRESS_FROM_DHCP);
			return 1;
		}
		_local_ip = Ethernet.localIP();
#if _DEBUG
		Serial.print("DHCP gave me this IP address: ");
#endif
	}
#if _DEBUG
	Ethernet.localIP().printTo(Serial);
	Serial.println();
	Serial.print("Start UDP connection on port: ");
	Serial.println(_local_port);
#endif
	_udp.begin(_local_port);
	return 0;
}

void UdpConnectionProvider_t::Reset() {
	_remote_ip = IPAddress();
	_remote_port = 0;
}

int UdpConnectionProvider_t::Connected() {
	if (!_time_out || millis() - _time_of_last_received_message > _time_out) {
		return 1;
	}
	return 0;
}

int UdpConnectionProvider_t::BeginPacket() {

#ifdef _DEBUG
	Serial.println("Begin packet to");
	_remote_ip.printTo(Serial);
	Serial.print(":");
	Serial.println(_remote_port);
#endif

	_udp.beginPacket(_remote_ip, _remote_port);
	return 0;
}

int UdpConnectionProvider_t::Write(void* buffer, unsigned int size) {
	if (!buffer || size == 0) {
		return 0;
	}

#ifdef _DEBUG
	Serial.print("Writing bytes: ");
	for (int i = 0; i < size; i++) {
		Serial.print(static_cast<int>(reinterpret_cast<char*>(buffer)[i]));
		Serial.print(' ');
	}
	Serial.println();
#endif

	_udp.write(static_cast<uint8_t*>(buffer), size);
	return 0;
}

int UdpConnectionProvider_t::EndPacket() {
	LOGLN("End packet");

	_udp.endPacket();
	return 0;
}

int UdpConnectionProvider_t::Send(void* buffer, unsigned int size) {

#ifdef _DEBUG
	LOG("Sending packet to ");
	_remote_ip.printTo(Serial);
	LOG(":");
	LOGLN(_remote_port);
#endif

	_udp.beginPacket(_remote_ip, _remote_port);
	_udp.write(static_cast<uint8_t*>(buffer), size);
	_udp.endPacket();
	return 0;
}

int UdpConnectionProvider_t::Receive(unsigned int& readed_bytes) {
	unsigned int packet_size = _udp.parsePacket();
	if (packet_size) {
		_time_of_last_received_message = millis();
		_remote_ip = _udp.remoteIP();
		_remote_port = _udp.remotePort();

		if (packet_size > _buffer_size) {
			ThrowException(Exceptions::EC_CP_MESSAGE_TOO_LARGE);
			return 1;
		}

		readed_bytes = _udp.read(static_cast<char*>(_buffer), _buffer_size);

#ifdef _DEBUG
		Serial.print("Bytes readed: ");
		Serial.println(readed_bytes);
		Serial.print("Bytes: ");

		for (int i = 0; i < packet_size; i++) {
			Serial.print(int(static_cast<char*>(_buffer)[i]));
			Serial.print(' ');
		}

		Serial.println();
		delay(200);
#endif
	} else {
		readed_bytes = 0;
	}
	return 0;
}

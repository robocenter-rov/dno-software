#pragma once

class ConnectionProvider_t {
protected:
	unsigned int _buffer_size;
	void* _buffer;
public:
	ConnectionProvider_t(unsigned int buffer_size);
	virtual ~ConnectionProvider_t();
	virtual int Begin() = 0;
	virtual void Reset() = 0;
	virtual int Connected() = 0;
	virtual int BeginPacket() = 0;
	virtual int Write(void* buffer, unsigned int size) = 0;
	virtual int Write(char c) = 0;
	virtual int Write(int i) = 0;
	virtual int Write(unsigned int i) = 0;
	virtual int EndPacket() = 0;
	virtual int Send(void* buffer, unsigned int size) = 0;
	virtual int Receive(unsigned int& readed_bytes) = 0;
	void* Buffer() const;
};

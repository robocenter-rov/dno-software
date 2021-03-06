#pragma once

class ConnectionProvider_t {
protected:
	unsigned int _buffer_size;
	char* _buffer;

public:
	ConnectionProvider_t(unsigned int buffer_size);
	virtual ~ConnectionProvider_t();
	virtual int Begin() = 0;
	virtual void Reset() = 0;
	virtual int Connected() = 0;
	virtual int BeginPacket() = 0;
	virtual int Write(void* buffer, unsigned int size) = 0;
	template <typename T>
	int Write(T val) {
		return Write(&val, sizeof(T));
	}
	virtual int EndPacket() = 0;
	virtual int Send(void* buffer, unsigned int size) = 0;
	virtual int Receive(unsigned int& readed_bytes) = 0;
	void* Buffer() const;
};

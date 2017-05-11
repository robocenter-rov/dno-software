#pragma once

#include "Debug.h"

class DataReader_t {
private:
	void* _buffer;
	unsigned int _buffer_size;
	unsigned int _readed;
public:
	DataReader_t(void* buffer, unsigned int buffer_size) : _buffer(buffer), _buffer_size(buffer_size), _readed(0) {}

	template<typename T>
	int Read(T& out) {
		if (_buffer_size - _readed < sizeof(T)) {
			return -1;
		}
		out = *reinterpret_cast<T*>(_buffer + _readed);
		_readed += sizeof(T);
		LOG("Reader, read: ");
		LOGLN(sizeof(T));
		return sizeof(T);
	}

	int ReadInt8AsFloat(float& f, float min, float max) {
		char v;
		int r = Read(v);
		if (r < 0) {
			return -1;
		}
		//Serial.print("Read: ");
		//Serial.println(int(v));
		//Serial.print("That is: ");
		f = (v + 128) / 255.f * (max - min) + min;
		//Serial.println(f);
		return 1;
	}

	unsigned int Available() const {
		return _buffer_size - _readed;
	}
};

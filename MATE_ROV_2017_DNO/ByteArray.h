#pragma once

#include "SharedPtr.h"
#include "Arduino.h"

class ByteArray_t  {
private:
	SharedPtr_t<byte> _ptr;
	unsigned int _size;
public:
	ByteArray_t(unsigned int size) : _ptr(new byte[size]), _size(size) {}
	ByteArray_t(void* source, unsigned int size) : _ptr(new byte[size]), _size(size) {
		memcpy(_ptr.Get(), source, _size);
	}
	template<typename T>
	ByteArray_t(const T& o) : _ptr(new byte[sizeof(T)]), _size(sizeof(T)) {
		memcpy(_ptr.Get(), &o, _size);
	}
	byte* Get() const { return _ptr.Get(); }
	unsigned int GetSize() const { return _size; }
};
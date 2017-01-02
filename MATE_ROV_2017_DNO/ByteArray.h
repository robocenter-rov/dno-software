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
	byte* Get() const { return _ptr.Get(); }
	unsigned int GetSize() const { return _size; }
};
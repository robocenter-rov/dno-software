#include "ConnectionProvider.h"

ConnectionProvider_t::ConnectionProvider_t(unsigned int buffer_size) {
	_buffer_size = buffer_size;
	_buffer = new char[buffer_size];
}

ConnectionProvider_t::~ConnectionProvider_t() {
	delete[] _buffer;
}

void* ConnectionProvider_t::Buffer() const {
	return _buffer;
}

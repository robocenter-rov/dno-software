#include "ConnectionProvider.h"

ConnectionProvider_t::ConnectionProvider_t(unsigned int buffer_size) {
	_buffer_size = buffer_size;
	_buffer = new char[buffer_size];
}

ConnectionProvider_t::~ConnectionProvider_t() {
	delete[] _buffer;
}

int ConnectionProvider_t::Write(unsigned int i) {
	return Write(static_cast<int>(i));
}

void* ConnectionProvider_t::Buffer() const {
	return _buffer;
}

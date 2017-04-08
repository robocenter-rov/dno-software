#pragma once

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
		out = *reinterpret_cast<T*>(_buffer);
		_readed += sizeof(T);
		return sizeof(T);
	}

	unsigned int Available() const {
		return _buffer_size - _readed;
	}
};
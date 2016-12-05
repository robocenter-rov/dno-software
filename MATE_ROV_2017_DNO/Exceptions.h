#pragma once

class Exception_t {
public:
	char* _message;
	Exception_t(char* message) {
		_message = message;
	}
};

class OutOfRangeException_t : public Exception_t {
public:
	using Exception_t::Exception_t;
};
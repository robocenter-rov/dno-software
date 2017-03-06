#include "../Utils/Debug.h"
#include "Exceptions.h"
#include "../Utils/Map.h"

Map_t<Exceptions::EXCEPTION_CODE, char*> ExceptionMessages;

List_t<Exceptions::Exception_t> Exceptions::_exception_list;

String Exceptions::Exception_t::GetFullMessage() const {
	return String(GetMessage(_exception)) + ", in file: " + _file +  ", at line: " + _line;
}

void Exceptions::Init() {
#define reg_except(code, name, message) ExceptionMessages.Add(code, message);
#include "../Exceptions/RegisterException/RegisterExceptions.h"
#undef reg_except;
}

void Exceptions::Throw(EXCEPTION_CODE exception, unsigned int line, const char* file) {

	LOG("Throwing exception: ");
	LOGLN(GetMessage(exception));

	_exception_list.PushBack(Exception_t(exception, line, file));
}

Exceptions::EXCEPTION_CODE Exceptions::GetLastCode() {
	return _exception_list.Empty() ? EC_NONE : _exception_list.Back().GetCode();
}

Exceptions::Exception_t* Exceptions::GetLastException() {
	return _exception_list.Empty() ? nullptr : &_exception_list.Back();
}

bool Exceptions::SmthngWrong() {
	return !_exception_list.Empty();
}

List_t<Exceptions::Exception_t>& Exceptions::GetList() {
	return _exception_list;
}

const char* Exceptions::GetMessage(EXCEPTION_CODE exception) {
	char* message;
	if (ExceptionMessages.Find(exception, message)) {
		return message;
	}
	return nullptr;
}

void Exceptions::Release() {

	LOGLN("Releasing exceptions");

	_exception_list.Clear();
}

#pragma once

#include "List.h"
#include "Arduino.h"

class Exceptions {
public:

	enum EXCEPTION_CODE {
#define reg_except(code, name, message) EC_##name = code,
#define reg_except_begin_fields_list
#define reg_except_field(type, name)
#define reg_except_field_last(type, name)
#define reg_except_end_fields_list
#include "RegisterExceptions.h"
#undef reg_except
#undef reg_except_begin_fields_list
#undef reg_except_field
#undef reg_except_field_last
#undef reg_except_end_fields_list
	};

	class Exception_t {
	protected:
		EXCEPTION_CODE _exception;
		int _line;
		const char* _file;
	public:
		Exception_t() {
			_exception = EC_NONE;
			_line = -1;
			_file = nullptr;
		}

		Exception_t(EXCEPTION_CODE exception, unsigned int line, const char* file)
			: 
			_exception(exception),
			_line(line),
			_file(file) {}

		virtual ~Exception_t() {}

		int GetLine() const { return _line; }
		const char* GetFile() const { return _file; }
		EXCEPTION_CODE GetCode() const { return _exception; }

		String GetFullMessage() const;
	};

	static void Init();
	static void Throw(EXCEPTION_CODE exception, unsigned int line, const char* file);
	static EXCEPTION_CODE GetLastCode();
	static Exception_t* GetLastException();
	static bool SmthngWrong();
	static List_t<Exception_t>& GetList();
	static const char* GetMessage(EXCEPTION_CODE exception);
	static void Release();

private:
	static List_t<Exception_t> _exception_list;
};

#define ThrowException(exception_code) Exceptions::Throw(exception_code, __LINE__, __FILE__)
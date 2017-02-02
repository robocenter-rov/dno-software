#pragma once
#include "Debug.h"

template<typename T>
class SelfExpandoContainer_t {
private:
	T* _data;
	unsigned int _size;
	SelfExpandoContainer_t(const SelfExpandoContainer_t& e) : _data(nullptr), _size(0) {}
public:
	SelfExpandoContainer_t() : _data(nullptr), _size(0) {
		LOGLN("New SelfExpandoContainer");
	}

	template<typename K>
	SelfExpandoContainer_t(const K& e) {
		_size = sizeof(K);
		_data = malloc(_size);
		LOG("New SelfExpandoContainer with size: ");
		LOGLN(sizeof(K));
		memcpy(_data, &e, _size);
	}

	~SelfExpandoContainer_t() {

		LOGLN("Free SelfExpandoContainer");

		if (_data) {
			free(_data);
		}
	}

	template<typename K>
	void operator=(const K& e) {
		if (_size < sizeof(K) || _size > sizeof(K) * 2) {
			if (_data) {
				free(_data);
				_data = nullptr;
			}

			LOG("SelfExpandoContainer: reallocate memory from ");
			LOG(_size);
			LOG(" bytes, to ");
			LOG(sizeof(K));
			LOGLN(" bytes");

			_data = static_cast<T*>(realloc(_data, sizeof(K)));
			_size = sizeof(K);
		}
		new (_data) K(e);
	}

	T* operator->() {
		return _data;
	}

	T* Get() const {
		return _data;
	}

	template<typename K>
	K& As() {
		return *static_cast<K*>(_data);
	}
};

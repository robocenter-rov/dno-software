#pragma once

template<typename T>
class SelfExpandoContainer_t {
private:
	T* _data;
	unsigned int _size;
	SelfExpandoContainer_t(const SelfExpandoContainer_t& e) : _data(nullptr), _size(0) {}
public:
	SelfExpandoContainer_t() : _data(nullptr), _size(0) {}

	~SelfExpandoContainer_t() {
		if (_data) {
			free(_data);
		}
	}

	template<typename K>
	void operator=(const K& e) {
		if (_size < sizeof(K) || _size > sizeof(K) * 2) {
			if (_data) {
				free(_data);
			}
			_data = static_cast<T*>(realloc(_data, sizeof(K)));
			_size = sizeof(K);
		}
		memcpy(_data, &e, sizeof(K));
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
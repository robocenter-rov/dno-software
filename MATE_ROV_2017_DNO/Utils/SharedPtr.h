#pragma once

template<typename T>
class SharedPtr_t {
protected:
	struct Node_t {
		T* ptr;
		unsigned int ref_count;
		Node_t(T* ptr, unsigned int ref_count = 0) {
			this->ptr = ptr;
			this->ref_count = ref_count;
		}
		~Node_t() {
			delete ptr;
		}
	};

	Node_t* _data;
public:
	SharedPtr_t() {
		_data = nullptr;
	}

	SharedPtr_t(T* ptr) {
		_data = new Node_t(ptr);
	}

	SharedPtr_t(const SharedPtr_t& e) {
		_data = e._data;
		++_data->ref_count;
	}

	~SharedPtr_t() {
		if (_data && --_data->ref_count == 0) {
			delete _data;
		}
	}

	T* Get() const {
		return _data->ptr;
	}
};

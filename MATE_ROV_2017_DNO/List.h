#pragma once

template <typename T>
class List_t {
public:
	class ListNode_t;
private:
	ListNode_t* _front;
	ListNode_t* _back;

	int _size;
public:
	class ListNode_t {
	private:
		List_t* _parent;
		ListNode_t* _next;
		ListNode_t* _prev;
		T _data;
	public:
		ListNode_t(const T& data, List_t* parent, ListNode_t* prev = nullptr, ListNode_t* next = nullptr) {
			_prev = prev;
			_next = next;

			_data = data;

			_parent = parent;

			if (prev) {
				prev->_next = this;
			} else if (_parent) {
				_parent->_front = this;
			}

			if (next) {
				next->_prev = this;
			} else if (_parent) {
				_parent->_back = this;
			}
		}

		~ListNode_t() {
			if (_prev) {
				_prev->_next = _next;
			}
			if (_next) {
				_next->_prev = _prev;
			}
			if (_parent) {
				if (_parent->_front == this) {
					_parent->_front = _next;
				}
				if (_parent->_back == this) {
					_parent->_back = _prev;
				}
				--_parent->_size;
			}
		}

		ListNode_t* GetNext() {
			return _next;
		}

		ListNode_t* GetPrev() {
			return _prev;
		}

		T& GetData() {
			return _data;
		}
	};

	List_t() {
		_back = _front = nullptr;
		_size = 0;
	}

	ListNode_t* PushFront(const T& data) {
		_size++;
		return new ListNode_t(data, this, nullptr, _front);
	}

	ListNode_t* PushBack(const T& data) {
		_size++;
		return new ListNode_t(data, this, _back);
	}

	T& Front() {
		return _front->GetData();
	}

	T& Back() {
		return _back->GetData();
	}

	ListNode_t* FrontNode() const {
		return _front;
	}

	ListNode_t* BackNode() const {
		return _back;
	}

	bool Empty() const {
		return !_size;
	}

	unsigned int Size() const {
		return _size;
	}

	void Clear() const {
		while (_front) {
			delete _front;
		}
	}

	static void InsertAfter(ListNode_t* after, ListNode_t* node) {
		
	}

};
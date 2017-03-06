#pragma once
#include "../Exceptions/Exceptions.h"

template<typename KEY, typename DATA>
class Map_t {
private:
	struct Node_t {
		KEY key;
		DATA data;
		Node_t() {}
		Node_t(KEY key, DATA data) {
			this->key = key;
			this->data = data;
		}
	};

	bool _prepared_for_queries;
	Node_t* _nodes;
	unsigned int _size;
	unsigned int _capacity;

	void PrepareForQueries() {
		qsort(_nodes, _size, sizeof(Node_t), [](const void* a, const void* b) -> int { 
			return static_cast<const Node_t*>(a)->key < static_cast<const Node_t*>(b)->key ? -1 :
				   static_cast<const Node_t*>(a)->key == static_cast<const Node_t*>(b)->key ? 0 : 1;
		});
		_prepared_for_queries = true;

#ifdef _DEBUG
		Serial.println("Map: preparing for queries");
		for (int i = 0; i < _size; i++) {
			Serial.print("Key: ");
			Serial.println(_nodes[i].key);
		}
#endif
	}

public:
	Map_t() : _prepared_for_queries(false), _nodes(nullptr), _size(0), _capacity(0) {}

	Map_t(int capacity) : _prepared_for_queries(false), _nodes(nullptr), _size(0), _capacity(capacity) {
		Reserve(_capacity);
	}

	~Map_t() {
		if (_nodes) {
			delete[] _nodes;
		}
	}

	void Add(KEY key, DATA data) {
		if (_size == _capacity) {
			Reserve(_capacity + 1);
		}

		_nodes[_size++] = Node_t(key, data);

		_prepared_for_queries = false;
	}

	bool Find(KEY key, DATA& out_data) {
		if (!_prepared_for_queries) {
			PrepareForQueries();
		}

		unsigned int first = 0;
		unsigned int last = _size;

		if (_size == 0) {
			return false;
		} else if (_nodes[0].key > key) {
			return false;
		} else if (_nodes[_size-1].key < key) {
			return false;
		}

		while (first < last) {
			unsigned int mid = first + (last - first) / 2;

			if (key <= _nodes[mid].key)
				last = mid;
			else
				first = mid + 1;
		}

		if (_nodes[last].key == key) {
			out_data = _nodes[last].data;
			return true;
		} else {
			return false;
		}
	}

	void Reserve(int capacity) {
		Node_t* _new_nodes = new Node_t[capacity];
		if (_nodes) {
			memcpy(_new_nodes, _nodes, sizeof(Node_t) * capacity);
			delete[] _nodes;
		}
		_nodes = _new_nodes;
		_capacity = capacity;
	}
}; 

#pragma once

inline uint32_t HashLy(char byte, uint32_t hash = 0) {
	return (hash * 1664525) + byte + 1013904223;
}

inline uint32_t HashLy(void* buffer, int size, uint32_t hash = 0) {
	auto b = reinterpret_cast<char*>(buffer);

	for (int i = 0; i < size; i++) {
		hash = HashLy(b[i], hash);
	}

	return hash;
}

template<typename T>
uint32_t HashLy(T val, uint32_t hash = 0) {
	return HashLy(&val, sizeof(T), hash);
}

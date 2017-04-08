#pragma once

inline uint32_t HashLy(char byte, uint32_t hash) {
	return (hash * 1664525) + byte + 1013904223;
}
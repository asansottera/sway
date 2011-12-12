// The code in this file is based on the public domain code available at
// http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogLookup

#ifndef SWAY_ILOG2_H
#define SWAY_ILOG2_H

#include <cstdint>

namespace sway {

template<class T>
T ilog2(T x);

static const uint8_t ilog2_lookup[256] = {
	0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7 };

template<>
inline uint8_t ilog2(uint8_t x) {
    return ilog2_lookup[x];
}

template<>
inline uint16_t ilog2(uint16_t x) {
	register uint16_t tmp;
	if ((tmp = x >> 8)) return 8 + ilog2_lookup[tmp];
    return ilog2_lookup[x];
}

template<>
inline uint32_t ilog2(uint32_t x) {
	register uint32_t tmp;
	if ((tmp = x >> 24)) return 24 + ilog2_lookup[tmp];
	if ((tmp = x >> 16)) return 16 + ilog2_lookup[tmp];
	if ((tmp = x >> 8)) return 8 + ilog2_lookup[tmp];
    return ilog2_lookup[x];
}

template<>
inline uint64_t ilog2(uint64_t x) {
	register uint64_t tmp;
	if ((tmp = x >> 56)) return 56 + ilog2_lookup[tmp];
	if ((tmp = x >> 48)) return 48 + ilog2_lookup[tmp];
	if ((tmp = x >> 40)) return 40 + ilog2_lookup[tmp];
	if ((tmp = x >> 32)) return 32 + ilog2_lookup[tmp];
	if ((tmp = x >> 24)) return 24 + ilog2_lookup[tmp];
	if ((tmp = x >> 16)) return 16 + ilog2_lookup[tmp];
	if ((tmp = x >> 8)) return 8 + ilog2_lookup[tmp];
    return ilog2_lookup[x];
}

}

#endif

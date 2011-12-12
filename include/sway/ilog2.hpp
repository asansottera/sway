// The code in this file is based on the public domain code available at
// http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogLookup

/*
Copyright (c) 2011, Andrea Sansottera
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
 - Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
 - Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef SWAY_ILOG2_HPP
#define SWAY_ILOG2_HPP

#include <boost/cstdint.hpp>

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

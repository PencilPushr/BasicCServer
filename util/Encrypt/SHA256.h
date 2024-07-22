#pragma once

#include <cstdint>

// Reference guide:
// https://en.wikipedia.org/wiki/SHA-2

//Note 1: All variables are 32 bit unsigned integers and addition is calculated modulo 232
//Note 2 : For each round, there is one round constant k[i] and one entry in the message schedule array w[i], 0 ? i ? 63
//Note 3 : The compression function uses 8 working variables, a through h
//Note 4 : Big - endian convention is used when expressing the constants in this pseudocode,
//and when parsing message block data from bytes to words, for example,
//the first word of the input message "abc" after padding is 0x61626380

class SHA256
{
public:
	SHA256();
	~SHA256() = default();

private:
	uint8_t m_data[64];
	uint32_t m_data_len;
	uint64_t m_bit_len;
	uint32_t m_hash_values[8];
};
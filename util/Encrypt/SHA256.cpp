#include "SHA256.h"

SHA256::SHA256()
{
	m_hash_values[0] = 0x6a09e667;
	m_hash_values[1] = 0xbb67ae85;
	m_hash_values[2] = 0x3c6ef372;
	m_hash_values[3] = 0xa54ff53a;
	m_hash_values[4] = 0x510e527f;
	m_hash_values[5] = 0x9b05688c;
	m_hash_values[6] = 0x1f83d9ab;
	m_hash_values[7] = 0x5be0cd19;
}



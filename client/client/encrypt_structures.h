#ifndef STRUCTURES_H
#define STRUCTURES_H
/*
	For this header file 3 things:
	1) const functions from Protocol AES for encrypt the data (from the internet)
	2) my own functions that connected to the encrypt and to the message format
	3) our connection to the stations (listening.. bing..)
*/

/* 1 */

// Encryption: Forward Rijndael S-box 
unsigned char sEncrypt[256] =
{
	0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76,
	0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0,
	0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15,
	0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75,
	0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84,
	0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF,
	0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8,
	0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2,
	0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73,
	0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB,
	0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79,
	0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08,
	0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A,
	0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E,
	0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF,
	0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16
};

// Encryption: Multiply by 2 for MixColumns
unsigned char mul2[] =
{
	0x00,0x02,0x04,0x06,0x08,0x0a,0x0c,0x0e,0x10,0x12,0x14,0x16,0x18,0x1a,0x1c,0x1e,
	0x20,0x22,0x24,0x26,0x28,0x2a,0x2c,0x2e,0x30,0x32,0x34,0x36,0x38,0x3a,0x3c,0x3e,
	0x40,0x42,0x44,0x46,0x48,0x4a,0x4c,0x4e,0x50,0x52,0x54,0x56,0x58,0x5a,0x5c,0x5e,
	0x60,0x62,0x64,0x66,0x68,0x6a,0x6c,0x6e,0x70,0x72,0x74,0x76,0x78,0x7a,0x7c,0x7e,
	0x80,0x82,0x84,0x86,0x88,0x8a,0x8c,0x8e,0x90,0x92,0x94,0x96,0x98,0x9a,0x9c,0x9e,
	0xa0,0xa2,0xa4,0xa6,0xa8,0xaa,0xac,0xae,0xb0,0xb2,0xb4,0xb6,0xb8,0xba,0xbc,0xbe,
	0xc0,0xc2,0xc4,0xc6,0xc8,0xca,0xcc,0xce,0xd0,0xd2,0xd4,0xd6,0xd8,0xda,0xdc,0xde,
	0xe0,0xe2,0xe4,0xe6,0xe8,0xea,0xec,0xee,0xf0,0xf2,0xf4,0xf6,0xf8,0xfa,0xfc,0xfe,
	0x1b,0x19,0x1f,0x1d,0x13,0x11,0x17,0x15,0x0b,0x09,0x0f,0x0d,0x03,0x01,0x07,0x05,
	0x3b,0x39,0x3f,0x3d,0x33,0x31,0x37,0x35,0x2b,0x29,0x2f,0x2d,0x23,0x21,0x27,0x25,
	0x5b,0x59,0x5f,0x5d,0x53,0x51,0x57,0x55,0x4b,0x49,0x4f,0x4d,0x43,0x41,0x47,0x45,
	0x7b,0x79,0x7f,0x7d,0x73,0x71,0x77,0x75,0x6b,0x69,0x6f,0x6d,0x63,0x61,0x67,0x65,
	0x9b,0x99,0x9f,0x9d,0x93,0x91,0x97,0x95,0x8b,0x89,0x8f,0x8d,0x83,0x81,0x87,0x85,
	0xbb,0xb9,0xbf,0xbd,0xb3,0xb1,0xb7,0xb5,0xab,0xa9,0xaf,0xad,0xa3,0xa1,0xa7,0xa5,
	0xdb,0xd9,0xdf,0xdd,0xd3,0xd1,0xd7,0xd5,0xcb,0xc9,0xcf,0xcd,0xc3,0xc1,0xc7,0xc5,
	0xfb,0xf9,0xff,0xfd,0xf3,0xf1,0xf7,0xf5,0xeb,0xe9,0xef,0xed,0xe3,0xe1,0xe7,0xe5
};

// Encryption: Multiply by 3 for MixColumns
unsigned char mul3[] =
{
	0x00,0x03,0x06,0x05,0x0c,0x0f,0x0a,0x09,0x18,0x1b,0x1e,0x1d,0x14,0x17,0x12,0x11,
	0x30,0x33,0x36,0x35,0x3c,0x3f,0x3a,0x39,0x28,0x2b,0x2e,0x2d,0x24,0x27,0x22,0x21,
	0x60,0x63,0x66,0x65,0x6c,0x6f,0x6a,0x69,0x78,0x7b,0x7e,0x7d,0x74,0x77,0x72,0x71,
	0x50,0x53,0x56,0x55,0x5c,0x5f,0x5a,0x59,0x48,0x4b,0x4e,0x4d,0x44,0x47,0x42,0x41,
	0xc0,0xc3,0xc6,0xc5,0xcc,0xcf,0xca,0xc9,0xd8,0xdb,0xde,0xdd,0xd4,0xd7,0xd2,0xd1,
	0xf0,0xf3,0xf6,0xf5,0xfc,0xff,0xfa,0xf9,0xe8,0xeb,0xee,0xed,0xe4,0xe7,0xe2,0xe1,
	0xa0,0xa3,0xa6,0xa5,0xac,0xaf,0xaa,0xa9,0xb8,0xbb,0xbe,0xbd,0xb4,0xb7,0xb2,0xb1,
	0x90,0x93,0x96,0x95,0x9c,0x9f,0x9a,0x99,0x88,0x8b,0x8e,0x8d,0x84,0x87,0x82,0x81,
	0x9b,0x98,0x9d,0x9e,0x97,0x94,0x91,0x92,0x83,0x80,0x85,0x86,0x8f,0x8c,0x89,0x8a,
	0xab,0xa8,0xad,0xae,0xa7,0xa4,0xa1,0xa2,0xb3,0xb0,0xb5,0xb6,0xbf,0xbc,0xb9,0xba,
	0xfb,0xf8,0xfd,0xfe,0xf7,0xf4,0xf1,0xf2,0xe3,0xe0,0xe5,0xe6,0xef,0xec,0xe9,0xea,
	0xcb,0xc8,0xcd,0xce,0xc7,0xc4,0xc1,0xc2,0xd3,0xd0,0xd5,0xd6,0xdf,0xdc,0xd9,0xda,
	0x5b,0x58,0x5d,0x5e,0x57,0x54,0x51,0x52,0x43,0x40,0x45,0x46,0x4f,0x4c,0x49,0x4a,
	0x6b,0x68,0x6d,0x6e,0x67,0x64,0x61,0x62,0x73,0x70,0x75,0x76,0x7f,0x7c,0x79,0x7a,
	0x3b,0x38,0x3d,0x3e,0x37,0x34,0x31,0x32,0x23,0x20,0x25,0x26,0x2f,0x2c,0x29,0x2a,
	0x0b,0x08,0x0d,0x0e,0x07,0x04,0x01,0x02,0x13,0x10,0x15,0x16,0x1f,0x1c,0x19,0x1a
};

// Used in KeyExpansion
unsigned char rconEn[256] = {
	0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
	0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
	0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
	0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
	0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
	0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
	0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
	0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
	0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
	0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
	0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
	0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
	0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
	0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
	0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
	0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d
};

// Auxiliary function for KeyExpansion
void KeyExpansionCore(unsigned char* in, unsigned char i) {
	// Rotate left by one byte: shift left 
	unsigned char t = in[0];
	in[0] = in[1];
	in[1] = in[2];
	in[2] = in[3];
	in[3] = t;

	// S-box 4 bytes 
	in[0] = sEncrypt[in[0]];
	in[1] = sEncrypt[in[1]];
	in[2] = sEncrypt[in[2]];
	in[3] = sEncrypt[in[3]];

	// RCon
	in[0] ^= rconEn[i];
}

using namespace std;


struct key_exchange
{
	std::string port;
	int prime;
	int primitive_root;
	int dh_key;
	int stationNum;
	int size;
};
#define SIGN '#'

uint64_t compute_dh_key2(uint64_t prime, uint64_t primitive_root, uint64_t secret) {
	uint64_t result = 1;
	primitive_root %= prime;
	while (secret > 0) {
		if (secret & 1) {
			result = (result * primitive_root) % prime;
		}
		secret = secret >> 1;
		primitive_root = (primitive_root * primitive_root) % prime;
	}
	return result;
}
void random()
{
	int thing = rand() + time(NULL);
	srand(thing);
}
// Function to generate a random prime number
int generate_prime() {
	// Randomly generate a number between 2 and 100
	random();
	int prime = rand() % 99 + 2;

	// Check if the number is prime
	for (int i = 2; i < prime; i++) {
		if (prime % i == 0) {
			// If the number is not prime, try again
			return generate_prime();
		}
	}

	// Return the prime number
	return prime;
}

// Function to generate a random primitive root of a given prime number
int generate_primitive_root(int prime) {
	// Randomly generate a number between 2 and the prime - 1
	random();
	int root = rand() % (prime - 2) + 2;
	int p;
	// pow(3, 4);
	 // Check if the number is a primitive root
	for (int i = 2; i < prime; i++) {
		p = pow(root, i);
		if (p % prime == 1) {
			// If the number is not a primitive root, try again
			return generate_primitive_root(prime);
		}
	}

	// Return the primitive root
	return root;
}

// Function to compute the Diffie-Hellman key
int compute_dh_key(int prime, int primitive_root, int secret) {
	// Compute the Diffie-Hellman key using the formula: key = (primitive_root ^ secret) % prime
	int to_key = pow(primitive_root, secret);
	int key = to_key % prime;
	return key;
}
std::string buildingMessage2d(struct key_exchange& s)
{
	std::string messageTemplate = SIGN + s.port + SIGN + to_string(s.prime) + SIGN + to_string(s.primitive_root) + SIGN + to_string(s.dh_key) + SIGN + to_string(s.stationNum) + SIGN + to_string(s.size) + SIGN;
	return messageTemplate;
}
int messageSize2d(struct key_exchange& s, std::string msg)
{
	int size = 0;
	size = s.port.length() + to_string(s.stationNum).length() + to_string(s.prime).length() + to_string(s.size).length() + to_string(s.dh_key).length() + to_string(s.primitive_root).length() + msg.length() + 5;
	cout << "size: " << size;
	return size;
}





std::string buildPATH(std::string rand_result)
{
	std::string fin;
	if (rand_result == "123")
	{
		fin = "5400*8000*2022*1111";
	}
	if (rand_result == "132")
	{
		fin = "5400*2022*8000*1111";
	}
	if (rand_result == "213")
	{
		fin = "8000*5400*2022*1111";
	}
	if (rand_result == "231")
	{
		fin = "8000*2022*5400*1111";
	}
	if (rand_result == "321")
	{
		fin = "2022*8000*5400*1111";
	}
	if (rand_result == "312")
	{
		fin = "2022*5400*8000*1111";
	}
	return fin;
}

/* The main KeyExpansion function
 * Generates additional keys using the original key
 * Total of 11 128-bit keys generated, including the original
 * Keys are stored one after the other in expandedKeys
 */
void KeyExpansionEn(unsigned char inputKey[16], unsigned char expandedKeys[176]) {
	// The first 128 bits are the original key
	for (int i = 0; i < 16; i++) {
		expandedKeys[i] = inputKey[i];
	}

	int bytesGenerated = 16; // Bytes we've generated so far
	int rconIteration = 1; // Keeps track of rcon value
	unsigned char tmpCore[4]; // Temp storage for core

	while (bytesGenerated < 176) {
		/* Read 4 bytes for the core
		* They are the previously generated 4 bytes
		* Initially, these will be the final 4 bytes of the original key
		*/
		for (int i = 0; i < 4; i++) {
			tmpCore[i] = expandedKeys[i + bytesGenerated - 4];
		}

		// Perform the core once for each 16 byte key
		if (bytesGenerated % 16 == 0) {
			KeyExpansionCore(tmpCore, rconIteration++);
		}

		for (unsigned char a = 0; a < 4; a++) {
			expandedKeys[bytesGenerated] = expandedKeys[bytesGenerated - 16] ^ tmpCore[a];
			bytesGenerated++;
		}

	}
}

/* Serves as the initial round during encryption
 * AddRoundKey is simply an XOR of a 128-bit block with the 128-bit key.
 */
void AddRoundKey(unsigned char* state, unsigned char* roundKey) {
	for (int i = 0; i < 16; i++) {
		state[i] ^= roundKey[i];
	}
}

/* Perform substitution to each of the 16 bytes
 * Uses S-box as lookup table
 */
void SubBytes(unsigned char* state) {
	for (int i = 0; i < 16; i++) {
		state[i] = sEncrypt[state[i]];
	}
}

// Shift left, adds diffusion
void EnShiftRows(unsigned char* state) {
	unsigned char tmp[16];

	/* Column 1 */
	tmp[0] = state[0];
	tmp[1] = state[5];
	tmp[2] = state[10];
	tmp[3] = state[15];

	/* Column 2 */
	tmp[4] = state[4];
	tmp[5] = state[9];
	tmp[6] = state[14];
	tmp[7] = state[3];

	/* Column 3 */
	tmp[8] = state[8];
	tmp[9] = state[13];
	tmp[10] = state[2];
	tmp[11] = state[7];

	/* Column 4 */
	tmp[12] = state[12];
	tmp[13] = state[1];
	tmp[14] = state[6];
	tmp[15] = state[11];

	for (int i = 0; i < 16; i++) {
		state[i] = tmp[i];
	}
}

/* MixColumns uses mul2, mul3 look-up tables
 * Source of diffusion
 */
void MixColumns(unsigned char* state) {
	unsigned char tmp[16];

	tmp[0] = (unsigned char)mul2[state[0]] ^ mul3[state[1]] ^ state[2] ^ state[3];
	tmp[1] = (unsigned char)state[0] ^ mul2[state[1]] ^ mul3[state[2]] ^ state[3];
	tmp[2] = (unsigned char)state[0] ^ state[1] ^ mul2[state[2]] ^ mul3[state[3]];
	tmp[3] = (unsigned char)mul3[state[0]] ^ state[1] ^ state[2] ^ mul2[state[3]];

	tmp[4] = (unsigned char)mul2[state[4]] ^ mul3[state[5]] ^ state[6] ^ state[7];
	tmp[5] = (unsigned char)state[4] ^ mul2[state[5]] ^ mul3[state[6]] ^ state[7];
	tmp[6] = (unsigned char)state[4] ^ state[5] ^ mul2[state[6]] ^ mul3[state[7]];
	tmp[7] = (unsigned char)mul3[state[4]] ^ state[5] ^ state[6] ^ mul2[state[7]];

	tmp[8] = (unsigned char)mul2[state[8]] ^ mul3[state[9]] ^ state[10] ^ state[11];
	tmp[9] = (unsigned char)state[8] ^ mul2[state[9]] ^ mul3[state[10]] ^ state[11];
	tmp[10] = (unsigned char)state[8] ^ state[9] ^ mul2[state[10]] ^ mul3[state[11]];
	tmp[11] = (unsigned char)mul3[state[8]] ^ state[9] ^ state[10] ^ mul2[state[11]];

	tmp[12] = (unsigned char)mul2[state[12]] ^ mul3[state[13]] ^ state[14] ^ state[15];
	tmp[13] = (unsigned char)state[12] ^ mul2[state[13]] ^ mul3[state[14]] ^ state[15];
	tmp[14] = (unsigned char)state[12] ^ state[13] ^ mul2[state[14]] ^ mul3[state[15]];
	tmp[15] = (unsigned char)mul3[state[12]] ^ state[13] ^ state[14] ^ mul2[state[15]];

	for (int i = 0; i < 16; i++) {
		state[i] = tmp[i];
	}
}

/* Each round operates on 128 bits at a time
 * The number of rounds is defined in AESEncrypt()
 */
void Round(unsigned char* state, unsigned char* key) {
	SubBytes(state);
	EnShiftRows(state);
	MixColumns(state);
	AddRoundKey(state, key);
}

// Same as Round() except it doesn't mix columns
void FinalRound(unsigned char* state, unsigned char* key) {
	SubBytes(state);
	EnShiftRows(state);
	AddRoundKey(state, key);
}

/* The AES encryption function
 * Organizes the confusion and diffusion steps into one function
 */
void AESEncrypt(unsigned char* message, unsigned char* expandedKey, unsigned char* encryptedMessage) {
	unsigned char state[16]; // Stores the first 16 bytes of original message
	for (int i = 0; i < 16; i++) {
		state[i] = message[i];
	}
	int numberOfRounds = 9;
	AddRoundKey(state, expandedKey); // Initial round
	for (int i = 0; i < numberOfRounds; i++) {
		Round(state, expandedKey + (16 * (i + 1)));
	}
	FinalRound(state, expandedKey + 160);

	// Copy encrypted state to buffer
	for (int i = 0; i < 16; i++) {
		encryptedMessage[i] = state[i];
	}
}

/* 2 */

/* The format of the message that sent to each station */
struct Sender
{
	std::string port;
	int stationNum;
	std::string id;
	int size;
	std::string data;
};

/*
	Function builds the message that will send to the stations.
	Input: struct - represent the format of the message template
	Output: string that soon will sent to one of the stations
*/
std::string buildingMessage(struct Sender& s)
{
	std::string messageTemplate = SIGN + s.port + SIGN + std::to_string(s.stationNum) + SIGN + s.id + SIGN + std::to_string(s.size) + SIGN + s.data + SIGN;
	std::cout << "\nbuildingMessage= " << messageTemplate;
	return messageTemplate;
}

/*
	Function that calculates the size of the message.
	Input: the full message as struct && the encrypt message
	Output: size of the full message as integer
*/
int messageSize(struct Sender& s, std::string msg)
{
	int size = 0;
	size = s.port.length() + std::to_string(s.stationNum).length() + s.id.length() + std::to_string(s.size).length() + msg.length() + 5;
	return size;
}

unsigned char* encryptTheMessage(unsigned char* paddedMessage, int paddedMessageLen, unsigned char* key)
{
	unsigned char* encryptedMessage = new unsigned char[paddedMessageLen];
	unsigned char expandedKey[176];

	KeyExpansionEn(key, expandedKey);

	for (int i = 0; i < paddedMessageLen; i += 16)
	{
		AESEncrypt(paddedMessage + i, expandedKey, encryptedMessage + i);
	}
	delete[] paddedMessage;
	std::cout << "-->" << encryptedMessage << "<--\n";
	return encryptedMessage;
}

unsigned char* padToSixteenBytes(char* char_userInput, unsigned char* key)
{
	// Pad message to 16 bytes
	int originalLen = strlen((const char*)char_userInput);
	int paddedMessageLen = originalLen;

	if ((paddedMessageLen % 16) != 0) {
		paddedMessageLen = (paddedMessageLen / 16 + 1) * 16;
	}

	unsigned char* paddedMessage = new unsigned char[paddedMessageLen];
	for (int i = 0; i < paddedMessageLen; i++) {
		if (i >= originalLen) {
			paddedMessage[i] = 0;
		}
		else {
			paddedMessage[i] = char_userInput[i];
		}
	}
	return encryptTheMessage(paddedMessage, paddedMessageLen, key);
}


/* 3 */
std::string get_path()
{
	std::string path;
	std::string ipAddress = "127.0.0.1";			// IP Address of the server
	int port = 8400;						// Listening port # on the server

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Can't start Winsock, Err #" << wsResult << std::endl;
		exit(0);
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		std::cerr << "Can't create socket, Err #" << WSAGetLastError() << std::endl;
		WSACleanup();
		exit(0);
	}

	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server, Err # \n--------PLEASE MAKE SURE THE SERVER IS RUNNING--------" << WSAGetLastError() << std::endl;
		closesocket(sock);
		WSACleanup();
		exit(0);
	}

	// Do-while loop to send and receive data
	char buf[4096];
	std::string userInput = "PATH REQUEST";

	if (userInput.size() > 0) // Make sure the user has typed in something
	{
		// Send the text
		int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
		if (sendResult != SOCKET_ERROR)
		{
			// Wait for response
			ZeroMemory(buf, 4096);
			int bytesReceived = recv(sock, buf, 4096, 0);
			if (bytesReceived > 0)
			{
				// Echo response to console
				std::cout << "Client> " << std::string(buf, 0, bytesReceived) << std::endl;
				path = std::string(buf, 0, bytesReceived);
			}
		}
	}
	// Gracefully close down everything
	closesocket(sock);
	WSACleanup();
	return path;
}

/* Defines will be here in the next sprint :) */
//ports: 1- 5000 2- 8080 3- 3400
std::string diffie_hellman_mode(std::string path)
{
	int shared_key1, shared_key2, shared_key3;
	string ipAddress = "127.0.0.1"; // IP Address of the server
	int port = 5000; // Listening port # on the server
	cout << "\n" << port;

	// Initialize WinSock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		std::cerr << "Can't start talking to proxy" << wsResult << endl;
		return "";
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
		WSACleanup();
		return "";
	}
	// Fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Can't connect to server, Err #" << WSAGetLastError() << " \n--------diffie hellman error - PLEASE MAKE SURE ALL STATIONS ARE RUNNING--------" << endl;
		closesocket(sock);
		WSACleanup();
		return "";
	}
	key_exchange messageTemplate; // Defining the structure

	// Inserts parameters into the structure
	messageTemplate.port = path;
	messageTemplate.prime = generate_prime();
	messageTemplate.primitive_root = generate_primitive_root(messageTemplate.prime);
	random();
	int secret;
	secret = rand();
	std::cout << "secret:" << secret;

	messageTemplate.dh_key = static_cast<int>(compute_dh_key2(static_cast<uint64_t>(messageTemplate.prime), static_cast<uint64_t>(messageTemplate.primitive_root), static_cast<uint64_t>(secret)));;
	messageTemplate.stationNum = 1;

	std::string msg = buildingMessage2d(messageTemplate);
	const char* msg_structure = msg.c_str(); // Changing the variable type (for using 'send' function)
	std::cout << "\n buffer: " << msg_structure;

	int msg_size = messageSize2d(messageTemplate, msg);
	int sendResult = send(sock, msg_structure, msg_size, 0); // Send the text

	char client_dh_key_str1[100];
	char client_dh_key_str2[100];
	char client_dh_key_str3[100];

	if (sendResult != SOCKET_ERROR)
	{
		// Wait for response
		ZeroMemory(client_dh_key_str1, 100);
		ZeroMemory(client_dh_key_str2, 100);
		ZeroMemory(client_dh_key_str3, 100);

		int bytesReceived = recv(sock, client_dh_key_str1, sizeof(client_dh_key_str1), 0);

		if (bytesReceived > 0)
		{
			// Echo response to console
			int client_dh_key = std::stoi(client_dh_key_str1);
			// Compute the final shared key
			shared_key1 = static_cast<int>(compute_dh_key2(static_cast<uint64_t>(messageTemplate.prime), static_cast<uint64_t>(client_dh_key), static_cast<uint64_t>(secret)));

			// Print the final shared key
			std::cout << "\nShared key1: " << shared_key1 << std::endl;
		}
		bytesReceived = recv(sock, client_dh_key_str2, sizeof(client_dh_key_str2), 0);

		if (bytesReceived > 0)
		{
			// Echo response to console
			int client_dh_key = std::stoi(client_dh_key_str2);
			// Compute the final shared key
			shared_key2 = static_cast<int>(compute_dh_key2(static_cast<uint64_t>(messageTemplate.prime), static_cast<uint64_t>(client_dh_key), static_cast<uint64_t>(secret)));

			// Print the final shared key
			std::cout << "\nShared key2: " << shared_key2 << std::endl;
		}

		bytesReceived = recv(sock, client_dh_key_str3, sizeof(client_dh_key_str3), 0);

		if (bytesReceived > 0)
		{
			// Echo response to console
			int client_dh_key = std::stoi(client_dh_key_str3);
			// Compute the final shared key
			shared_key3 = static_cast<int>(compute_dh_key2(static_cast<uint64_t>(messageTemplate.prime), static_cast<uint64_t>(client_dh_key), static_cast<uint64_t>(secret)));

			// Print the final shared key
			std::cout << "\nShared key3: " << shared_key3 << endl;
		}
	}
	std::string result = std::to_string(shared_key1) + " " + std::to_string(shared_key2) + " " + std::to_string(shared_key3);
	std::cout << "\n" << result << "\n" << std::endl;
	return result;
}

std::string buildKey(int x)
{
	char hex_str[100];
	std::string hex_key;
	std::stringstream stream;
	stream << std::setfill('0') << std::hex;
	for (int i = 0; i < 16; i++) {
		stream.str("");
		int value = x - i;
		if (value < 0) { value = x + i; }
		stream << std::setw(2) << value;
		hex_key += stream.str() + " ";
	}
	std::cout << "\nHex key: " << hex_key << std::endl;
	return hex_key;
}
#endif /* STRUCTURES_H */
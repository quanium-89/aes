#ifndef _H_AES
#define _H_AES

#include <stdint.h>

#define AES_NB  4
#define MAX_NK  8
#define MAX_NR  14

#define _AES_BLOCK_SIZE  (AES_NB << 2)

typedef struct {
	uint8_t rd_key[(MAX_NR + 1) * AES_NB * 4];
	uint8_t inv_rd_key[(MAX_NR + 1) * AES_NB * 4];
	int rounds;
} _AES_KEY;

void expand_key(_AES_KEY *key, void *_init_key, int bits);
void _AES_encrypt(void *in, void *out, _AES_KEY *key);
void _AES_encrypt(void *in, void *out, _AES_KEY *key);

#endif

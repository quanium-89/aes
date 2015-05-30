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

typedef struct aes_st {
	_AES_KEY key;
	uint8_t ivec[_AES_BLOCK_SIZE];
	void (*encrypt)(void *in, void *out, struct aes_st *ctx);
	void (*decrypt)(void *in, void *out, struct aes_st *ctx);
} _AES_CTX;


void expand_key(_AES_KEY *key, void *_init_key, int bits);

void _AES_ecb_encrypt(void *in, void *out, _AES_CTX *ctx);
void _AES_ecb_decrypt(void *in, void *out, _AES_CTX *ctx);
void _AES_cbc_encrypt(void *in, void *out, _AES_CTX *ctx);
void _AES_cbc_decrypt(void *in, void *out, _AES_CTX *ctx);

#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/aes.h>
#include <sys/time.h>
#include "aes.h"
#include "util.h"

#define KEY  "hello world!"
#define BITS  256

void print_elapsed_time(struct timeval *tv_start, struct timeval *tv_end)
{
	long sec, usec;

	sec = tv_end->tv_sec - tv_start->tv_sec;
	usec = tv_end->tv_usec - tv_start->tv_usec;
	usec += sec * 1000000;
	printf("[*]: %ld\n", usec);
}

int main()
{
	SHA256_CTX md_ctx;
	AES_KEY key;
	_AES_CTX ctx;
	uint8_t buf[64], nb[32], enb[32];
	int i, len;
	int nk = BITS >> 5, nr = 6 + nk, key_nb = (nr + 1) * AES_NB * 4;
	FILE *fp, *ofp;
	struct timeval tv_start, tv_end;

	for (len = 0; len < 64; len += 32) {
		SHA256_Init(&md_ctx);
		if (len == 0) {
			SHA256_Update(&md_ctx, KEY, strlen(KEY));
			SHA256_Final(buf, &md_ctx);
		} else {
			SHA256_Update(&md_ctx, buf + len - 32, 32);
			SHA256_Update(&md_ctx, KEY, strlen(KEY));
			SHA256_Final(buf + len, &md_ctx);
		}
	}

	AES_set_encrypt_key(buf, BITS, &key);
	hex_dump((uint8_t *)key.rd_key, key_nb);

	expand_key(&(ctx.key), buf, BITS);
	ctx.encrypt = _AES_ecb_encrypt;
	ctx.decrypt = _AES_ecb_decrypt;
	hex_dump(ctx.key.rd_key, key_nb);


	for (i = 0; i < 32; i++)
		nb[i] = '\0';

	gettimeofday(&tv_start, NULL);
	for (i = 0; i < 1024; i++)
		AES_encrypt(nb, enb, &key);
	gettimeofday(&tv_end, NULL);
	print_elapsed_time(&tv_start, &tv_end);
	hex_dump(enb, 16);

	gettimeofday(&tv_start, NULL);
	for (i = 0; i < 1024; i++)
		ctx.encrypt(nb, enb, &ctx);
	gettimeofday(&tv_end, NULL);
	print_elapsed_time(&tv_start, &tv_end);
	hex_dump(enb, 16);

	for (i = 0; i < 32; i++)
		nb[i] = enb[i];
	ctx.decrypt(nb, enb, &ctx);
	hex_dump(enb, 16);


	if ((fp = fopen("test.bin", "r")) == NULL) {
		fprintf(stderr, "File open failed\n");
		exit(-1);
	}

	while (!feof(fp)) {
		if (fread(nb, 1, _AES_BLOCK_SIZE, fp) == 0) {
			fprintf(stderr, "Failed in call to fread()\n");
			exit(-1);
		}
	}

	fseek(fp, 0, SEEK_SET);
	i = 0;
	gettimeofday(&tv_start, NULL);
	while (!feof(fp)) {
		++i;
		if (fread(nb, 1, _AES_BLOCK_SIZE, fp) == 0) {
			fprintf(stderr, "Failed in call to fread()\n");
			exit(-1);
		}
		AES_encrypt(nb, enb, &key);
	}
	gettimeofday(&tv_end, NULL);
	print_elapsed_time(&tv_start, &tv_end);
	printf("blocks = %d\n", i);

	fseek(fp, 0, SEEK_SET);
	i = 0;
	gettimeofday(&tv_start, NULL);
	while (!feof(fp)) {
		++i;
		if (fread(nb, 1, _AES_BLOCK_SIZE, fp) == 0) {
			fprintf(stderr, "Failed in call to fread()\n");
			exit(-1);
		}
		ctx.encrypt(nb, enb, &ctx);
	}
	gettimeofday(&tv_end, NULL);
	print_elapsed_time(&tv_start, &tv_end);
	printf("blocks = %d\n", i);

	fclose(fp);


	fp = fopen("test.enc", "r");
	ofp = fopen("tmp.bin", "w");
	ctx.encrypt = _AES_cbc_encrypt;
	ctx.decrypt = _AES_cbc_decrypt;
	memcpy(ctx.ivec, buf + 32, _AES_BLOCK_SIZE);
	while (!feof(fp)) {
		if (fread(nb, 1, _AES_BLOCK_SIZE, fp) == 0) {
			fprintf(stderr, "Failed in call to fread()\n");
			exit(-1);
		}
		ctx.decrypt(nb, enb, &ctx);
		fwrite(enb, 1, _AES_BLOCK_SIZE, ofp);
	}
	fclose(fp);
	fclose(ofp);

	return 0;
}

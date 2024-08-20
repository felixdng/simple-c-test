#ifndef __CRYPTO_H
#define __CRYPTO_H


enum {
	TP_MD5 = 0,
	TP_SHA256,
	TP_SHA512,
	TP_HASH,

	CRYP_MAX
};
typedef struct {
	unsigned char data[256];
	char str[512 + 1];
} cryp_t;

const char *get_type_str(int type);
int get_type_int(const char *type);
int crypto(int type, void *data, unsigned int len, cryp_t *code);

typedef void * (*FN_TYPE)();
typedef struct {
	void *data;
	unsigned int size;
	unsigned int cnt;
	FN_TYPE fn_cmp;
	FN_TYPE fn_print;
	void *check;
} hash_t;

int cryp_hash(hash_t *hs);

#endif /* __CRYPTO_H */

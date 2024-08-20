#include <stdio.h>
#include <string.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#include <openssl/lhash.h>

#include "crypto.h"


static const char *types_str[] = {
	"md5",
	"sha256",
	"sha512",
	"hash",
	"null"
};

const char *get_type_str(int type)
{
	if (type >= CRYP_MAX)
		return NULL;
	return types_str[type];
}

int get_type_int(const char *type)
{
	int i;
	for (i = 0; i < CRYP_MAX; ++i) {
		if (0 == strncmp(type, types_str[i], strlen(types_str[i])))
			break;
	}
	return i;
}

static unsigned char *_HASH(const unsigned char *d, size_t n, unsigned char *md)
{
	unsigned long r = lh_strhash((const char *)d);
	unsigned long tmp = r;
	unsigned char *pmd = md;
	printf("%s:%lx\n", __func__, r);

	unsigned long base = 0x100;
	while (tmp /= base)
		pmd++;
	tmp = r;
	do {
		if (tmp % base) {
			*pmd-- = tmp % base;
		}
		tmp /= base;
	} while (tmp);
	return md;
}

int crypto(int type, void *data, unsigned int len, cryp_t *code)
{
	if (type >= CRYP_MAX) return -1;
	if (NULL == data || NULL == code) return -1;
	if (len <= 0) return -1;

	memset(code, 0, sizeof(cryp_t));
	switch (type) {
		case TP_MD5: MD5((const unsigned char *)data, len, code->data);
			break;
		case TP_SHA256: SHA256((const unsigned char *)data, len, code->data);
			break;
		case TP_SHA512: SHA512((const unsigned char *)data, len, code->data);
			break;
		case TP_HASH: _HASH((const unsigned char *)data, len, code->data);
			break;
		default:
			return -1;
	}

	int i;
	for (i = 0; i < sizeof(code->data); ++i) {
		if (0 == code->data[i]) break;
		sprintf(&code->str[i * 2], "%02x", code->data[i]);
	}
	return 0;
}

int cryp_hash(hash_t *hs)
{
	if (NULL == hs) return -1;
	if (NULL == hs->data ||
		0 == hs->size ||
		0 == hs->cnt ||
		NULL == hs->fn_cmp)
		return -1;

	_LHASH *h = NULL;
//	h = lh_new(NULL, (LHASH_COMP_FN_TYPE)hs->fn_cmp);
	h = lh_new(NULL, (OPENSSL_LH_COMPFUNC)hs->fn_cmp);
	if (NULL == h) {
		fprintf(stderr, "lh_new() failed!\n");
		return -1;
	}

	int i;
	for (i = 0; i < hs->cnt; ++i)
		lh_insert(h, hs->data + i * hs->size);

	if (hs->fn_print) {
//		lh_doall(h, (LHASH_DOALL_FN_TYPE)hs->fn_print);
		lh_doall(h, (OPENSSL_LH_DOALL_FUNC)hs->fn_print);
	}

	if (hs->check) {
		void *data = lh_retrieve(h, hs->check);
		if (NULL == data)
			fprintf(stderr, "lh_retrieve() failed!\n");
		else if (hs->fn_print) {
			fprintf(stdout, "check:\n");
			hs->fn_print(data);
		}
	}

	lh_free(h);
	return 0;
}

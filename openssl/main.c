#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "crypto.h"


void pr_hex(const unsigned char *str, unsigned int len)
{
	int line = 16;
	int i;

	printf("[hex]\n");
	if (len <= 0) return;
	for (i = 0; i < line; ++i) {
		if (0 == i) printf(">:");
		printf("%02d ", i);
	}
	printf("\n");

	for (i = 0; i < len; ++i) {
		if (0 == str[i]) break;
		if (((i + 1) % line) == 1)
			printf("  ");
		printf("%02x ", str[i]);
		if (((i + 1) % line) == 0)
			printf("\n");
	}
	if ((i % line) != 0)
		printf("\n");
}

/* test for hash */
typedef struct {
	char name[32];
	int age;
	int no;
} person_t;

static int per_cmp(const void *a, const void *b)
{
	const person_t *pa = (const person_t *)a;
	const person_t *pb = (const person_t *)b;
#if 0
	return strcmp(pa->name, pb->name);
#else
	if (pa->age < pb->age)
		return -1;
	else if (pa->age > pb->age)
		return 1;
	return 0;
#endif
}

static void per_print(void *a)
{
	const person_t *pa = (const person_t *)a;
	printf("name:%s, age:%d, no:%d\n", 
			pa->name, pa->age, pa->no);
}

int test_hash(void)
{
	int i;
	person_t pers[10];
	for (i = 0; i < 10; ++i) {
		snprintf(pers[i].name, sizeof(pers[i].name), "%c-hoo", 'a' + i);
		pers[i].age = 10 + i;
		pers[i].no = i + 1;
	}

	person_t check;
	memcpy(&check, &pers[3], sizeof(person_t));

	hash_t hs = {
		.data = pers,
		.size = sizeof(person_t),
		.cnt = 10,
		.fn_cmp = (FN_TYPE)per_cmp,
		.fn_print = (FN_TYPE)per_print,
		.check = (void *)&check,
	};
	return cryp_hash(&hs);
}

int main(int argc, char *argv[])
{
	char str[128] = {};
	cryp_t code = {};

	if (2 == argc && 0 == strncmp(argv[1], "hash", 4)) {
		return test_hash();
	}

	if (argc != 3) {
		fprintf(stderr, "Usage:%s <type> <string>\n", argv[0]);
		fprintf(stderr, "  types:\n");
		int i;
		for (i = 0; i < CRYP_MAX; ++i)
			fprintf(stderr, "    %s\n", get_type_str(i));
		exit(1);
	}
	snprintf(str, sizeof(str), "%s", argv[2]);
	unsigned int len = strlen((char *)str);

	int type = get_type_int(argv[1]);

	if (0 == crypto(type, (void *)str, len, &code)) {
		pr_hex(code.data, sizeof(code.data));
		printf("[str] %s\n", code.str);
	}

	return 0;
}


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "type.h"
#include "list.h"


typedef struct test_hash {
	int key;
	struct hlist_node hnode;
} test_hash_t;

struct hlist_head *call_hash(struct hlist_head *hash, int id)
{
	unsigned int val = id % 8;
	return &hash[val];
}

int main(int argc, char *argv[])
{
	struct hlist_head *hash;
	struct hlist_node *p = NULL, *n = NULL;
	int i = 0;
	test_hash_t *pnode;

	hash = (struct hlist_head *)malloc(sizeof(struct hlist_head) * 8);
	if (NULL == hash) return -1;

	for (i = 0; i < 8; ++i) {
		INIT_HLIST_HEAD(&hash[i]);
	}

	for (i = 0; i < 100; ++i) {
		pnode = (test_hash_t *)calloc(1, sizeof(test_hash_t));
		if (NULL == pnode) continue;

		INIT_HLIST_NODE(&pnode->hnode);
		pnode->key = i + 1;
		hlist_add_head(&pnode->hnode, call_hash(hash, i));
	}

	int key = (2 == argc) ? atoi(argv[1]) : -1;
	for (i = 0; i < 8; ++i) {
		struct hlist_head *h = NULL;
		if (-1 != key) h = call_hash(hash, key);
		else h = &hash[i];
		hlist_for_each_safe(p, n, h) {
			pnode = hlist_entry(p, struct test_hash, hnode);
			if (NULL != pnode)
				printf("%d\n", pnode->key);
		}
		if (-1 != key) break;
	}



	return 0;
}


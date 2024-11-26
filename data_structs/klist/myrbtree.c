#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "myrbtree.h"
#include "rbtree_augmented.h"


static int my_cmp(const char *str1, const char *str2)
{
	unsigned long d1, d2;
	if (is_digit_string(str1, d1) && is_digit_string(str2, d2)) {
		return d1 < d2 ? -1 : d1 > d2 ? 1 : 0;
	} else {
		return strcmp(str1, str2);
	}
}

struct mytype *my_search(struct rb_root *root, const char *string)
{
	struct rb_node *node = root->rb_node;
	while (node) {
		struct mytype *data = container_of(node, struct mytype, node);
		int result = my_cmp(string, data->keystring);
		if (result < 0)
			node = node->rb_left;
		else if (result > 0)
			node = node->rb_right;
		else
			return data;
	}
	return NULL;
}

bool my_insert(struct rb_root *root, struct mytype *data)
{
	struct rb_node **new = &(root->rb_node);
	struct rb_node *parent = NULL;
	while (*new) {
		struct mytype *this = container_of(*new, struct mytype, node);
		int result = my_cmp(data->keystring, this->keystring);

		parent = *new;
		if (result < 0)
			new = &((*new)->rb_left);
		else if (result > 0)
			new = &((*new)->rb_right);
		else
			return false;
	}

	rb_link_node(&data->node, parent, new);
	rb_insert_color(&data->node, root);
	return true;
}

bool my_erase(struct rb_root *root, const char *string)
{
	struct mytype *data = my_search(root, string);
	if (data) {
		rb_erase(&data->node, root);
		return true;
	}
	return false;
}

void mytype_print(struct rb_root *root, int mode)
{
	struct rb_node *node;
	printf("list ---->:\n");
	for (node = rb_first(root); node; node = rb_next(node))
		printf("key=%s\n", rb_entry(node, struct mytype, node)->keystring);
}

void mytype_preorder(struct rb_node *node)
{
	if (NULL == node) return;
	printf("[%-5s] %s", rb_is_black(node) ? "black" : "red", 
		rb_entry(node, struct mytype, node)->keystring);
	if (node->rb_left)
		printf(", left:%s", rb_entry(node->rb_left, struct mytype, node)->keystring);
	else printf(", left:null");
	if (node->rb_right)
		printf(", right:%s", rb_entry(node->rb_right, struct mytype, node)->keystring);
	else printf(", right:null");
	printf("\n");

	if (node->rb_left) mytype_preorder(node->rb_left);
	if (node->rb_right) mytype_preorder(node->rb_right);
}


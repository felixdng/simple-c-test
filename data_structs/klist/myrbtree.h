#ifndef __MY_RBTREE_H
#define __MY_RBTREE_H


#include "type.h"
#include "rbtree.h"

struct mytype {
	char *keystring;
	struct rb_node node;
};

struct mytype *my_search(struct rb_root *root, const char *string);
bool my_insert(struct rb_root *root, struct mytype *data);
bool my_erase(struct rb_root *root, const char *string);
void mytype_print(struct rb_root *root, int mode);
void mytype_preorder(struct rb_node *node);

#endif /* __MY_RBTREE_H */

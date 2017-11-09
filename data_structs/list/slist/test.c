#include <stdio.h>
#include <stdlib.h>
#include "slist.h"

void show(pslist_t h)
{
	pslist_t p = h;
	printf("-----> ");
	while (NULL != p->next) {
		printf("%d, ", p->next->data.id);
		p = p->next;
	}
	printf("\n");
}

int main(int argc, char **argv)
{
	pslist_t h = NULL;
	pslist_t node;
	data_t data;
	int i;

	h = slist_init();
	if (!h) {
		perror("slist_init failed!\n");
		exit(1);
	}

	for (i = 0; i < 4; i++) {
		data.id = i;
		node = slist_new_node(data);
		slist_add(h, node);
	}

	printf("\ninit--------------\n");
	printf("len: %d\n", slist_length(h));
	show(h);


	data.id = 3;
	node = slist_new_node(data);
	slist_del(h, node);
	free(node);

	printf("\ndel--------------\n");
	printf("len: %d\n", slist_length(h));
	show(h);

	slist_clear(h);
	printf("\nclr--------------\n");
	printf("len: %d\n", slist_length(h));
	show(h);

	slist_destory(h);
	return 0;
}


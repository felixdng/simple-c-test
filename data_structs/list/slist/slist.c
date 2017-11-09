#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "slist.h"


int data_cmp(data_t d1, data_t d2)
{
	if (d1.id == d2.id) {
		return 0;
	} else {
		return 1;
	}
}

void data_cpy(data_t *dest, data_t *src)
{
	dest->id = src->id;
}

pslist_t slist_init(void)
{
	pslist_t h = (pslist_t)malloc(sizeof(slist_t));
	if (NULL == h) {
		return NULL;
	}

	memset(h, 0, sizeof(slist_t));
	h->next = NULL;
	return h;
}

/**
 * insert new node after pos node
 * pos : pos node
 * new : new node
 */
inline void _slist_add(pslist_t pos, pslist_t new)
{
	new->next = pos->next;
	pos->next = new;
}

/**
 * delete the node after pos
 * pos : pos node
 */
inline void _slist_del(pslist_t pos)
{
	pslist_t dest = pos->next;
	pos->next = dest->next;
	free(dest);
	dest = NULL;
}

void slist_add(pslist_t head, pslist_t new)
{
	pslist_t p = head;

	while (NULL != p->next)
		p = p->next;

	_slist_add(p, new);
}

void slist_del(pslist_t head, pslist_t obj)
{
	pslist_t p = head;
	pslist_t tmp = NULL;

	while (NULL != p->next) {
		tmp = p->next;
		if (!data_cmp(tmp->data, obj->data)) {
			_slist_del(p);
			break;
		}
		p = p->next;
	}
}

int slist_length(pslist_t head)
{
	int count = 0;
	pslist_t p = head;
	while (NULL != p->next) {
		count++;
		p = p->next;
	}

	return count;
}

pslist_t slist_new_node(data_t data)
{
	pslist_t p = (pslist_t)malloc(sizeof(slist_t));
	if (NULL == p)
		return NULL;

	data_cpy(&p->data, &data);
	return p;
}

void slist_clear(pslist_t head)
{
	pslist_t p = head;

	while (NULL != p->next) {
		_slist_del(p);
	}
}

void slist_destory(pslist_t head)
{
	if (head) {
		slist_clear(head);
		free(head);
		head = NULL;
	}
}


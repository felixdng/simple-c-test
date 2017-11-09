#ifndef __SLIST_H
#define __SLIST_H

typedef struct _data {
	int id;
} data_t;

typedef struct _slist {
	data_t data;
	struct _slist *next;
} slist_t, *pslist_t;

extern pslist_t slist_init(void);
extern void slist_add(pslist_t head, pslist_t new);
extern void slist_del(pslist_t head, pslist_t obj);
extern int slist_length(pslist_t head);
extern pslist_t slist_new_node(data_t data);
extern void slist_clear(pslist_t head);
extern void slist_destory(pslist_t head);

#endif

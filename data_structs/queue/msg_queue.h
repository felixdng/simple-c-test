#ifndef __MSG_QUEUE_H__
#define __MSG_QUEUE_H__

#include <stdbool.h>

typedef enum _QUEUE_STATUS {
	QUEUE_OK = 0,
	QUEUE_FAILED = -1,
	QUEUE_FULL = -2,
	QUEUE_EMPTY = -3,
} QUEUE_STATUS;

typedef struct _msg_buffer {
	int type;
	char *text;
} msg_buffer_t;

#undef QUEUE_SIZE
#define QUEUE_SIZE	5
typedef struct _msg_queue {
	msg_buffer_t data[QUEUE_SIZE];
	int front, rear;
} msg_queue_t, *msg_queue_p;

extern bool is_empty_queue(msg_queue_p queue);
extern bool is_full_queue(msg_queue_p queue);
extern QUEUE_STATUS create_msg_queue(msg_queue_p *pqueue);
extern void destroy_msg_queue(msg_queue_p *pqueue);
extern QUEUE_STATUS msg_queue_send(msg_queue_p queue, const void *msg, int length);
extern QUEUE_STATUS msg_queue_recv(msg_queue_p queue, void *msg, int length);

#endif /* __MSG_QUEUE_H__ */


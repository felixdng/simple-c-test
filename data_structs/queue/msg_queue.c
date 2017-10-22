/*
 * msg_queue.c
 **/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "msg_queue.h"

#define DEBUG
#ifdef DEBUG
#define PRINT_DEBUG(format, args...) \
	fprintf(stdout, "[%s] [debug] : "format"\n", __func__, ##args)
#else
#define PRINT_DEBUG(format, args...)
#endif

#define PRINT_ERR(format, args...) \
	fprintf(stderr, "[%s] [err] : "format"\n", __func__, ##args)


bool is_empty_queue(msg_queue_p queue)
{
	if (queue->front == queue->rear)
		return true;
	else
		return false;
}

bool is_full_queue(msg_queue_p queue)
{
	if (queue->front == ((queue->rear + 1) % QUEUE_SIZE))
		return true;
	else
		return false;
}

QUEUE_STATUS create_msg_queue(msg_queue_p *pqueue)
{
	int i;

	msg_queue_p queue = (msg_queue_p)malloc(sizeof(msg_queue_t));
	if (!queue) {
		PRINT_ERR("malloc failed!");
		return QUEUE_FAILED;
	}

	queue->front = queue->rear = QUEUE_SIZE - 1;

	for (i = 0; i < QUEUE_SIZE; i++) {
		queue->data[i].type = 0;
		queue->data[i].text = NULL;
	}

	*pqueue = queue;

	return QUEUE_OK;
}

void destroy_msg_queue(msg_queue_p *pqueue)
{
	int i;

	msg_queue_p queue = *pqueue;

	if (!is_empty_queue(queue)) {
		for (i = (queue->front + 1) % QUEUE_SIZE;
				i != (queue->rear + 1) % QUEUE_SIZE;
				i = (i+1) % QUEUE_SIZE) {
			queue->data[i].type = 0;
			free(queue->data[i].text);
			queue->data[i].text = NULL;
		}
	}

	free(queue);
	*pqueue = NULL;
}


QUEUE_STATUS msg_queue_send(msg_queue_p queue, const void *msg, int length)
{
	int old_rear = queue->rear;

	if (is_full_queue(queue)) {
		PRINT_ERR("msg queue is full.");
		return QUEUE_FULL;
	}

	queue->rear = (queue->rear + 1) % QUEUE_SIZE;

	queue->data[queue->rear].text = (char *)malloc(length * sizeof(char));
	if (!queue->data[queue->rear].text) {
		PRINT_ERR("malloc failed");
		queue->rear = old_rear;
		return QUEUE_FAILED;
	}

	memcpy(&(queue->data[queue->rear].type), msg, sizeof(queue->data[queue->rear].type));

	memcpy(queue->data[queue->rear].text, msg + sizeof(queue->data[queue->rear].type), length);

	return QUEUE_OK;
}

QUEUE_STATUS msg_queue_recv(msg_queue_p queue, void *msg, int length)
{
	if (is_empty_queue(queue)) {
		PRINT_ERR("msg queue is empty.");
		return QUEUE_EMPTY;
	}

	queue->front = (queue->front + 1 ) % QUEUE_SIZE;

	memcpy(msg, &(queue->data[queue->front].type), sizeof(queue->data[queue->front].type));
	memcpy(msg + sizeof(queue->data[queue->front].type), queue->data[queue->front].text, length);

	queue->data[queue->front].type = 0;
	free(queue->data[queue->front].text);
	queue->data[queue->front].text = NULL;

	return QUEUE_OK;
}


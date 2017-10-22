#include <stdio.h>
#include <string.h>

#include "msg_queue.h"

struct msgbuf {
	int type;
	char text[30];
};

static int test_func_1(void)
{
	int i;

	printf("\n------------ create_msg_queue --------------\n");
	msg_queue_p msgque;
	if (QUEUE_OK != create_msg_queue(&msgque)) {
		return -1;
	}
	printf("------------ create_msg_queue end --------------\n\n");

	printf("\n------------ msg_queue_send --------------\n");
	struct msgbuf buff;
	for (i = 0; i < 6; i++) {
		buff.type = i;
		sprintf(buff.text, "[msg %d]", i);
		msg_queue_send(msgque, &buff, sizeof(buff));
	}
	printf("------------ msg_queue_send end --------------\n\n");

	if (is_full_queue(msgque))
		printf("~~~ full queue ~~~\n");

	printf("\n------------ msg_queue_recv --------------\n");
	for (i = 0; i < 6; i++) {
		msg_queue_recv(msgque, &buff, sizeof(buff));
		printf("type: %d, text: %s\n", buff.type, buff.text);
	}
	printf("------------ msg_queue_recv end --------------\n\n");

	if (is_empty_queue(msgque))
		printf("### empty queue ###\n");

	printf("\n------------ destroy_msg_queue --------------\n");
	destroy_msg_queue(&msgque);
	printf("------------ destroy_msg_queue end --------------\n\n");

	return 0;
}

/****************************************************************/
static int test_func_2(void)
{
	
	return 0;
}

int main(int argc, char **argv)
{
	test_func_1();
	test_func_2();
	return 0;
}


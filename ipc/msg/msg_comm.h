#ifndef __MSG_COMM_H
#define __MSG_COMM_H

#define MSG_BUFF_SZ 1024
typedef struct _msg_buff {
	long int type;
	char data[MSG_BUFF_SZ];
} msg_buff_t;

typedef struct _msg_info {
	int msg_id;
	msg_buff_t buff;
} msg_info_t;

int msg_init(msg_info_t *info, int key);
int msg_dest(msg_info_t *info);
int msg_send(msg_info_t *info);
int msg_recv(msg_info_t *info);

#endif /* __MSG_COMM_H */

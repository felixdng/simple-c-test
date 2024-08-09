#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "msg_comm.h"

int msg_init(msg_info_t *info, int key)
{
	memset(info, 0, sizeof(msg_info_t));
	info->msg_id = msgget((key_t)key, 0664 | IPC_CREAT);
	if (info->msg_id < 0) {
		perror("msgget failed!\n");
		return -1;
	}
	return 0;
}

int msg_dest(msg_info_t *info)
{
	if (info->msg_id >= 0) {
		if (-1 == msgctl(info->msg_id, IPC_RMID, 0)) {
			perror("msgctl(IPC_RMID) failed!\n");
			return -1;
		}
		info->msg_id = -1;
	}
	return 0;
}

int msg_send(msg_info_t *info)
{
	if (-1 == msgsnd(info->msg_id, (void *)&info->buff, MSG_BUFF_SZ, 0)) {
		perror("msgsnd failed!\n");
		return -1;
	}
	return 0;
}

int msg_recv(msg_info_t *info)
{
	int ret = msgrcv(info->msg_id, (void *)&info->buff, MSG_BUFF_SZ, info->buff.type, 0);
	if (-1 == ret) {
		perror("msgrcv failed!\n");
		return -1;
	}
	return ret;
}


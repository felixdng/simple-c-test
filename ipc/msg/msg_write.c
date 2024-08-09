#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "msg_comm.h"

int main(int argc, char *argv[])
{
	msg_info_t msginfo;
	if (0 != msg_init(&msginfo, 0x1234))
		return -1;

	msginfo.buff.type = 1;

	while (fgets(msginfo.buff.data, MSG_BUFF_SZ, stdin)) {
		msg_send(&msginfo);
		sleep(1);
		if (0 == strncmp(msginfo.buff.data, "end", 3))
			break;
	}
	msg_dest(&msginfo);
}


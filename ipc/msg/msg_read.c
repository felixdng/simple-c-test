#include <stdio.h>
#include <string.h>

#include "msg_comm.h"

int main(int argc, char *argv[])
{
	msg_info_t msginfo;
	if (0 != msg_init(&msginfo, 0x1234))
		return -1;

	msginfo.buff.type = 1;
	while (1) {
		int ret = msg_recv(&msginfo);
		if (ret < 0) break;
		if (0 == ret) continue;
		if (0 == strncmp("end", msginfo.buff.data, 3)) break;
		printf("<type:%ld> recv[%d]: %s\n", msginfo.buff.type, ret, msginfo.buff.data);
	}

	msg_dest(&msginfo);
	return 0;
}


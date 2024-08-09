#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "shm_basic.h"


int main(int argc, char *argv[])
{
	int key = 0;
	int loop = 0;
	int finish = 0;

	if (argc >= 2)
		key = strtoul(argv[1], NULL, 0);
	if (argc >= 3)
		loop = strtoul(argv[2], NULL, 0);
	if (argc >= 4)
		finish = strtoul(argv[3], NULL, 0);

	if (key <= 0 || loop <= 0) {
		printf("\nUsage:\n");
		printf("%s <key> <loop> [anyting]\n\n", argv[0]);
		return -1;
	}

	shm_info_t shmi;
	if (-1 == shm_init(&shmi, key, 0, 0))
		return -1;

	while (-1 == finish) {
		char cmd[20] = {};
		fgets(cmd, sizeof(cmd), stdin);
		int ret = shm_put(&shmi, cmd, strlen(cmd) + 1);
		printf("ret:%d len:%d\n", ret, (int)(strlen(cmd) + 1));
		shm_dump(&shmi);
		if (0 == strncmp(cmd, "exit", 4)) return 0;
	}

	char spid[9] = {};
	snprintf(spid, sizeof(spid), "%d", getpid());
	char data[19];
	unsigned int num = 0;

	while (num < loop) {
		memset(data, 0, sizeof(data));
		if (((loop - 1) == num) && finish)
			sprintf(data, "exit-%s-%d", spid, num);
		else
			sprintf(data, "data-%s-%d", spid, num);
		int ret = shm_put(&shmi, data, strlen(data) + 1);
		if (-1 == ret) goto exit;
		printf("[%d] ret:%d len:%d %s\n", num, ret, (int)(strlen(data) + 1), data);
		shm_dump(&shmi);
		int msec = getrand(1000, 10);
		usleep(1000 * msec);
		num++;
	}

exit:
//	while (fgetc(stdin) != '\n');
	shm_dest(&shmi);
	return 0;
}


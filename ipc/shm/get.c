#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdlib.h>

#include "shm_basic.h"


static void pr_data(const char *pdata, unsigned int size)
{
	const int line_max = 15;
	int flag = 0;
	int idx;
	for (idx = 0; idx < size; ++idx) {
		if (isgraph(pdata[idx]))
			printf("%c ", pdata[idx]);
		else
			printf("#%x ", pdata[idx]);
		flag = 1;
		if (idx > 0 && ((idx % line_max) == 0)) {
			printf("\n");
			flag = 0;
		}
	}
	if (flag)
		printf("\n");
}

int main(int argc, char *argv[])
{
	int key = 0;
	unsigned int bsz = 0;
	unsigned int bcnt = 0;

	if (2 == argc) {
		key = strtoul(argv[1], NULL, 0);
	} else if (4 == argc) {
		key = strtoul(argv[1], NULL, 0);
		bsz = strtoul(argv[2], NULL, 0);
		bcnt = strtoul(argv[3], NULL, 0);
	}

	if (key <= 0) {
		printf("\nUsage:\n");
		printf("%s <key> [<block_size> <block_count>]\n\n", argv[0]);
		return -1;
	}

	shm_info_t shmi;
	if (-1 == shm_init(&shmi, key, bsz, bcnt))
		return -1;

	char data[16];
	while (1) {
		int msec = getrand(1000, 20);
		usleep(1000 * msec);
		//while (fgetc(stdin) != '\n');
		shm_dump(&shmi);
		int ret = shm_get(&shmi, data, sizeof(data));
		if (-1 == ret) goto exit;
		printf("ret:%d, data:\n", ret);
		pr_data(data, sizeof(data));

		if (0 == strncmp(data, "exit", 4)) break;
	}

exit:
	shm_dest(&shmi);
	return 0;
}


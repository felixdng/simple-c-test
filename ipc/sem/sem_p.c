#include <stdio.h>

#include "sem_comm.h"


int main(int argc, char *argv[])
{
	int sid1 = sem_init(KEY_1, -1);
	if (-1 == sid1) return -1;
	int sid2 = sem_init(KEY_2, -1);
	if (-1 == sid2) return -1;

	int num = 10;
	while (num-- > 0) {
		if (0 == sem_p(sid1)) {
			printf("sem p (%d)\n", num);
			sem_v(sid2);
		}
	}

	return 0;
}


#include <stdio.h>
#include <unistd.h>

#include "sem_comm.h"


int main(int argc, char *argv[])
{
	int sid1 = sem_init(KEY_1, 0);
	if (-1 == sid1) return -1;
	int sid2 = sem_init(KEY_2, 0);
	if (-1 == sid2) {
		sem_dest(sid1);
		return -1;
	}

	int num = 10;
	while (num-- > 0) {
		sleep(2);
		if (0 == sem_v(sid1)) {
			printf("sem v (%d)\n", num);
			sem_p(sid2);
		}
	}

	sem_dest(sid1);
	sem_dest(sid2);
	return 0;
}


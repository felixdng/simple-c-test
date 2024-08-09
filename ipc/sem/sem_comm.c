#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

typedef union semun{
    int val;
}semun;

int sem_init(int key, int init)
{
	int sem_id = semget((key_t)key, 1, 0664 | IPC_CREAT);
	if (-1 == sem_id) {
		printf("semget(%d) failed!\n", key);
		return -1;
	}

	if (init >= 0) {
		semun sem_union = { .val = init};
		if (-1 == semctl(sem_id, 0, SETVAL, sem_union)) {
			printf("semctl(SETVAL %d) failed!\n", sem_id);
			return -1;
		}
	}

	return sem_id;
}

int sem_dest(int sem_id)
{
	if (-1 == semctl(sem_id, 0, IPC_RMID)) {
		printf("semctl(IPC_RMID %d) failed!\n", sem_id);
		return -1;
	}
	return 0;
}

int sem_p(int sem_id)
{
	struct sembuf sbuf = {
		.sem_num = 0,
		.sem_op = -1,
		.sem_flg = SEM_UNDO,
	};
	if (-1 == semop(sem_id, &sbuf, 1)) {
		printf("semop(P %d) failed!\n", sem_id);
		return -1;
	}
	return 0;
}

int sem_v(int sem_id)
{
	struct sembuf sbuf = {
		.sem_num = 0,
		.sem_op = 1,
		.sem_flg = SEM_UNDO,
	};
	if (-1 == semop(sem_id, &sbuf, 1)) {
		printf("semop(V %d) failed!\n", sem_id);
		return -1;
	}
	return 0;
}


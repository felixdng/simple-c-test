#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "sem_basic.h"


typedef union semun {
	int val;
} semun;

int sem_init(sem_info_t *si, int key, int init)
{
	if (NULL == si) return -1;

	si->created = init < 0 ? 0 : 1;
	int semflag = si->created ? (0664 | IPC_CREAT) : 0;
	si->semid = semget((key_t)key, 1, semflag);
	if (-1 == si->semid) {
		printf("semget(0x%x) failed!\n", key);
		return -1;
	}

	if (si->created) {
		printf("sem create(%d) ok.\n", si->semid);
		semun sem_union = { .val = init};
		if (-1 == semctl(si->semid, 0, SETVAL, sem_union)) {
			printf("semctl(SETVAL %d) failed!\n", si->semid);
			semctl(si->semid, 0, IPC_RMID);
			si->semid = -1;
			si->created = 0;
			return -1;
		}
		printf("sem setval(%d) ok.\n", init);
	}

	return si->created;
}

int sem_dest(sem_info_t *si)
{
	if (NULL == si) return -1;
	if (si->created) {
		if (-1 == semctl(si->semid, 0, IPC_RMID)) {
			printf("semctl(IPC_RMID %d) failed!\n", si->semid);
			return -1;
		}
		si->created = 0;
		printf("sem rmid(%d) ok.\n", si->semid);
	}
	si->semid = -1;
	return 0;
}

int sem_p(sem_info_t *si)
{
	if (NULL == si) return -1;
	struct sembuf sbuf = {
		.sem_num = 0,
		.sem_op = -1,
		.sem_flg = si->created ? SEM_UNDO : 0,
	};
	if (-1 == semop(si->semid, &sbuf, 1)) {
		printf("semop(P %d) failed!\n", si->semid);
		return -1;
	}
	return 0;
}

int sem_v(sem_info_t *si)
{
	if (NULL == si) return -1;
	struct sembuf sbuf = {
		.sem_num = 0,
		.sem_op = 1,
		.sem_flg = si->created ? SEM_UNDO : 0,
	};
	if (-1 == semop(si->semid, &sbuf, 1)) {
		printf("semop(V %d) failed!\n", si->semid);
		return -1;
	}
	return 0;
}

int sem_check(int key)
{
	int semid = semget((key_t)key, 1, 0664);
	return (-1 == semid) ? -1 : 0;
}

int sem_getv(int sem_id)
{
	return semctl(sem_id, 0, GETVAL);
}

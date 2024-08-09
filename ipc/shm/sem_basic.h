#ifndef __SEM_BASIC_H
#define __SEM_BASIC_H


typedef struct _sem_info {
	int          semid;
	unsigned int created;
} sem_info_t;

int sem_init(sem_info_t *si, int key, int init);
int sem_dest(sem_info_t *si);
int sem_p(sem_info_t *si);
int sem_v(sem_info_t *si);
int sem_check(int key);
int sem_getv(int sem_id);

#endif /* __SEM_BASIC_H */

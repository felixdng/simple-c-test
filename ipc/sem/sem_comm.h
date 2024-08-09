#ifndef __SEM_COMM_H
#define __SEM_COMM_H

#define KEY_1 0x1001
#define KEY_2 0x1002

int sem_init(int key, int init);
int sem_dest(int sem_id);
int sem_p(int sem_id);
int sem_v(int sem_id);

#endif /* __SEM_COMM_H */

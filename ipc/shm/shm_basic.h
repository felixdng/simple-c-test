#ifndef __SHM_BASIC_H
#define __SHM_BASIC_H

#include "sem_basic.h"

typedef struct _shm_info {
	int               shmid;
	void              *addr;   /* shm address */
	unsigned int      created;
	sem_info_t        semi_mutex;
	sem_info_t        semi_empty;
	sem_info_t        semi_full;
} shm_info_t;

int shm_init(shm_info_t *shmi, int key, unsigned int blk_sz, unsigned int blk_cnt);
void shm_dest(shm_info_t *shmi);
int shm_put(shm_info_t *shmi, const char *pdata, unsigned int len);
int shm_get(shm_info_t *shmi, char *pdata, unsigned int size);
void shm_dump(shm_info_t *shmi);

int getrand(int max, int min);

#endif /* __SHM_BASIC_H */

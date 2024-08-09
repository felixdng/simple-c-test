#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>

#include "sem_basic.h"
#include "shm_basic.h"


#define pr_info(fmt, ...) \
printf("<shm info> "fmt"\n", ##__VA_ARGS__)

typedef struct _shm_data {
	unsigned int      magic;
	unsigned int      blk_sz;  /* block size */
	unsigned int      blk_cnt; /* block count */
	unsigned int      id_wt;
	unsigned int      id_rd;
} shm_data_t;

static shm_info_t *shm_info_init(shm_info_t *shmi)
{
	if (NULL == shmi) return NULL;
	memset(shmi, 0, sizeof(shm_info_t));
	shmi->shmid = -1;
	return shmi;
}

int shm_init(shm_info_t *shmi, int key, unsigned int blk_sz, unsigned int blk_cnt)
{
	if (NULL == shmi)
		return -1;
	shm_info_init(shmi);

	shmi->created = (0 == blk_sz || 0 == blk_cnt) ? 0 : 1;
	int shmflag = shmi->created ? (0664 | IPC_CREAT) : 0;
	unsigned int shmsize = shmi->created ? (blk_sz * blk_cnt + sizeof(shm_data_t)) : 0;

	shmi->shmid = shmget((key_t)key, shmsize, shmflag);
	if (-1 == shmi->shmid) {
		pr_info("shmget(key:%x) failed!", key);
		return -1;
	}

	shmi->addr = shmat(shmi->shmid, NULL, 0);
	if ((void *)-1 == shmi->addr) {
		pr_info("shmat(id:%d) failed!", shmi->shmid);
		goto exit1;
	}

	int semval;
	semval = shmi->created ? 1 : -1;
	if (-1 == sem_init(&shmi->semi_mutex, key + 0, semval))
		goto exit2;

	semval = shmi->created ? blk_cnt : -1;
	if (-1 == sem_init(&shmi->semi_empty, key + 1, semval))
		goto exit3;

	semval = shmi->created ? 0 : -1;
	if (-1 == sem_init(&shmi->semi_full, key + 2, semval))
		goto exit4;

	if (shmi->created) {
		pr_info("shm create(%d) ok.", shmi->shmid);
		shm_data_t *shmd = (shm_data_t *)shmi->addr;
		shmd->magic = 0;
		shmd->blk_sz = blk_sz;
		shmd->blk_cnt = blk_cnt;
		shmd->id_wt = 0;
		shmd->id_rd = 0;
	}
	pr_info("shm info: key=%x, blk_sz=%d, blk_cnt=%d", key, blk_sz, blk_cnt);
	return shmi->created;

exit4:
	sem_dest(&shmi->semi_empty);
exit3:
	sem_dest(&shmi->semi_mutex);
exit2:
	shmdt(shmi->addr);
	shmi->addr = NULL;
exit1:
	if (shmi->created) {
		shmctl(shmi->shmid, IPC_RMID, 0);
		shmi->created = 0;
	}
	shmi->shmid = -1;
	return -1;
}

void shm_dest(shm_info_t *shmi)
{
	if (NULL == shmi) return;

	sem_dest(&shmi->semi_full);
	sem_dest(&shmi->semi_empty);
	sem_dest(&shmi->semi_mutex);

	if (NULL != shmi->addr) {
		shmdt(shmi->addr);
		shmi->addr = NULL;
	}

	if (shmi->created) {
		shmctl(shmi->shmid, IPC_RMID, 0);
		shmi->created = 0;
		pr_info("shm rm(%d) ok.", shmi->shmid);
	}
	shmi->shmid = -1;
}

int shm_put(shm_info_t *shmi, const char *pdata, unsigned int len)
{
	if (NULL == shmi) return -1;

	if (-1 == sem_p(&shmi->semi_empty)) return -1;

	if (-1 == sem_p(&shmi->semi_mutex)) return -1;
	shm_data_t *shmd = (shm_data_t *)shmi->addr;
	unsigned wlen = len < shmd->blk_sz ? len : shmd->blk_sz;
	void *wdata = shmi->addr + sizeof(shm_data_t) + shmd->id_wt * shmd->blk_sz;
	memset(wdata, 0, shmd->blk_sz);
	memcpy(wdata, pdata, wlen);
	shmd->id_wt = (shmd->id_wt + 1) % shmd->blk_cnt;
	shmd->magic++;
	if (-1 == sem_v(&shmi->semi_mutex)) return -1;

	if (-1 == sem_v(&shmi->semi_full)) return -1;
	return wlen;
}

int shm_get(shm_info_t *shmi, char *pdata, unsigned int size)
{
	if (NULL == shmi) return -1;

	if (-1 == sem_p(&shmi->semi_full)) return -1;

	if (-1 == sem_p(&shmi->semi_mutex)) return -1;
	shm_data_t *shmd = (shm_data_t *)shmi->addr;
	unsigned int rlen = size > shmd->blk_sz ? shmd->blk_sz : size;
	void *rdata = shmi->addr + sizeof(shm_data_t) + shmd->id_rd * shmd->blk_sz;
	memset(pdata, 0, size);
	memcpy(pdata, rdata, rlen);
	shmd->id_rd = (shmd->id_rd + 1) % shmd->blk_cnt;
	if (-1 == sem_v(&shmi->semi_mutex)) return -1;

	if (-1 == sem_v(&shmi->semi_empty)) return -1;
	return rlen;
}

void shm_dump(shm_info_t *shmi)
{
	if (NULL == shmi) return;
	time_t tm;
	time(&tm);
	printf("[%ld] shm info.\n", tm);
	printf("  id:%d, addr:%p, created:%d\n", shmi->shmid, shmi->addr, shmi->created);
	printf("  [sem mutex] id:%d, created:%d, value:%d\n", 
			shmi->semi_mutex.semid, shmi->semi_mutex.created, 
			sem_getv(shmi->semi_mutex.semid));
	printf("  [sem empty] id:%d, created:%d, value:%d\n", 
			shmi->semi_empty.semid, shmi->semi_empty.created, 
			sem_getv(shmi->semi_empty.semid));
	printf("  [sem full] id:%d, created:%d, value:%d\n", 
			shmi->semi_full.semid, shmi->semi_full.created, 
			sem_getv(shmi->semi_full.semid));
	shm_data_t *shmd = (shm_data_t *)shmi->addr;
	printf("  [shm data] magic:%d blk_sz:%d, blk_cnt:%d, id_wt:%d, id_rd:%d\n",
			shmd->magic, shmd->blk_sz, shmd->blk_cnt, shmd->id_wt, shmd->id_rd);
	int iblk, ibyte;
	for (iblk = 0; iblk < shmd->blk_cnt; ++iblk) {
		char *ptr = shmi->addr + sizeof(shm_data_t) + iblk * shmd->blk_sz;
		char strb[32] = {};
		snprintf(strb, sizeof(strb), "  [block-%08d] ", iblk);
		printf("%s", strb);
		int flag = 0;
		for (ibyte = 0; ibyte < shmd->blk_sz; ++ibyte) {
			if (isgraph(ptr[ibyte])) printf("%c ", ptr[ibyte]);
			else printf("#%x ", ptr[ibyte]);
			flag = 1;
			if (ibyte > 0 && (0 == (ibyte % 15))) {
				printf("\n");
				flag = 0;
				if ((shmd->blk_sz - 1) != ibyte) {
					int len = strlen(strb);
					while (len-- > 0) printf(" ");
				}
			}
		}
		if (flag) printf("\n");
	}
}

int getrand(int max, int min)
{
	if (0 == max || max <= min) return 0;
	srand((unsigned int)time(NULL));
	int ret = rand() % max + min;
//	printf("%s: max=%d, min=%d, rand=%d\n",
//			__func__, max, min, ret);
	return ret;
}


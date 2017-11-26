#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>


static int wait_c = 0;
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void waitting(void)
{
	pthread_mutex_lock(&lock);
	wait_c = 0;
	while (0 == wait_c)
		pthread_cond_wait(&cond, &lock);
	pthread_mutex_unlock(&lock);
}

int waitting_timed(int second)
{
	struct timeval now;
	struct timespec timeout;
	int ret = 0;

	pthread_mutex_lock(&lock);
	gettimeofday(&now, NULL);
	timeout.tv_sec = now.tv_sec + second;
	timeout.tv_nsec = now.tv_usec * 1000;

	ret = 0;
	wait_c = 0;
	while (0 == wait_c && ret != ETIMEDOUT)
		ret = pthread_cond_timedwait(&cond, &lock, &timeout);
	pthread_mutex_unlock(&lock);

	return ret;
}

void resume(void)
{
	pthread_mutex_lock(&lock);
	wait_c = 1;
	if (1 == wait_c)
		pthread_cond_signal(&cond);
	pthread_mutex_unlock(&lock);
}

void *thrd_func(void *arg)
{
	printf("waitting...\n");
#if 0
	waitting();
#else
	while (ETIMEDOUT == waitting_timed(3))
		printf("== timeout!\n");
#endif
	printf("## stop wait!\n");
}

int main(int argc, char **argv)
{
	pthread_t tid;

	/**
	 * 将线程的属性设置为分离属性,
	 * 则线程结束后由系统自动回收线程占用的资源
	 */
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	if (pthread_create(&tid, &attr, thrd_func, NULL) != 0) {
		printf("pthread_create failed!\n");
		exit(1);
	}

	int i;
	for (i = 0; i < 5; ++i) {
		sleep(1);
		printf("sleep %d\n", i);
	}

	resume();

	sleep(3);
	return 0;
}

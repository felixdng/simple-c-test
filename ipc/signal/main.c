#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

void *thread_func(void *arg)
{
	while (1) {
		sleep(3);
		raise(SIGUSR1);
	}
}

void signal_handler(int sig_no)
{
	switch (sig_no) {
	case SIGUSR1:
		printf("catch signal\n");
		break;
	}
}

int main(int argc, char **argv)
{
	pthread_t tid;
	pthread_attr_t attr;

	signal(SIGUSR1, signal_handler);

	//设置子线程的属性为分离属性
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	if (pthread_create(&tid, &attr, thread_func, NULL) != 0) {
		perror("pthread_create");
		exit(1);
	}

	while (1)
		pause();

	return 0;
}


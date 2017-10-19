#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>


static void signal_handler(int signo)
{
	switch (signo) {
	case SIGALRM:
		printf("Caught the SIGALRM signal\n");
		break;
	}
}

int main(void)
{
	/* set signal handler */
	signal(SIGALRM, signal_handler);

	/* set time: 2 sec */
	alarm(2);

	while(1)
		sleep(10);

	return 0;
}

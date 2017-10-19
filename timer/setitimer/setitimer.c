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

	/* struct itimerval {
	 *     struct timeval it_interval; //定时间隔,初始化为零则不会定时
	 *     struct timeval it_value; //延时时长,初始化为零则不会触发信号
	 * }
	 *
	 * struct timeval {
	 *     time_t      tv_sec;  // seconds
	 *     suseconds_t tv_usec; // microseconds
	 * }
	 *
	 * settimer工作机制是, 先对it_value倒计时,当it_value为零时触发信号,
	 * 然后将it_interval的值复制给it_value,继续对it_value倒计时,一直这样循环下去.
	 *
	 * 2种应用模式:
	 * >1. 定时模式:
	 *      it_interval设置为定时间隔，it_value设置为第一次的延时时长
	 * >2. 延时模式:
	 *      it_interval设置为0，it_value设置为延时时长
	 *
	 * 取消定时设置: it_interval设置为0，it_value设置为0 
	 *
	 * */
	/* set time: 2 sec */
	struct itimerval val;
	val.it_value.tv_sec = 2;
	val.it_value.tv_usec = 0;
	val.it_interval.tv_sec = 0;
	val.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &val, NULL);

	while(1)
		sleep(10);

	return 0;
}

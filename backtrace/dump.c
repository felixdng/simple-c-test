#if 0
//backtrace 函数说明
#include <execinfo.h>
/*
 * 功能描述： 获取当前堆栈地址
 * 参数说明： buffer - 函数地址数组(传出)
 *          size   - 数组的最大长度 
 * 返 回 值：实际调用的层数
 */
int backtrace(void **buffer, int size);    

/*
 * 功能描述： 将地址转换成符号字符串数组
 * 参数说明： buffer - 函数地址数组
 *          size   - 有效地址的个数 
 * 返 回 值： 符号字符串数组指针（需要调用者释放）
 */
char **backtrace_symbols(void *const *buffer, int size);

/*
 * 功能描述： 将地址转换成符号字符串数组并写入指定文件
 * 参数说明： buffer - 函数地址数组
 *          size   - 有效地址的个数 
 *          fd     - 文件描述符
 * 返 回 值： 无
 */
void backtrace_symbols_fd(void *const *buffer, int size, int fd);
#endif


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <signal.h>	    /* for signal */
#include <execinfo.h> 	/* for backtrace() */
#include <fcntl.h>
#include <sys/types.h>

static struct
{
    int  sig_num;
    char sig_name[20];
} g_sig_arr[] = {
	{SIGILL,         "SIGILL",    },
	{SIGBUS,         "SIGBUS",    },
	{SIGFPE,         "SIGFPE",    },
	{SIGABRT,        "SIGABRT",   },
	{SIGSEGV,        "SIGSEGV",   },
	{SIGPIPE,        "SIGPIPE",   },
	{SIGTERM,        "SIGTERM",   },
	{SIGSTKFLT,      "SIGSTKFLT", },
	{SIGXFSZ,        "SIGXFSZ",   },
	{SIGTRAP,        "SIGTRAP",   },
};


static int open_file(FILE **pfp)
{
	if (NULL == pfp)
		return -1;

	time_t curr_time;
	time(&curr_time);
	struct tm *curr_tm = localtime(&curr_time);

	char fname[256] = { 0 };
	snprintf(fname, sizeof(fname), "core.%d-%d-%d_%d_%d_%d",
			curr_tm->tm_year + 1900, curr_tm->tm_mon + 1, curr_tm->tm_mday,
			curr_tm->tm_hour, curr_tm->tm_min, curr_tm->tm_sec);
	FILE *fp = fopen(fname, "w");
	if (NULL == fp)
		return -1;

	char buffer[4096] = {0};
	int count = readlink("/proc/self/exe", buffer, sizeof(buffer));
	if (count > 0) {
		buffer[count] = '\n';
		buffer[count + 1] = 0;
		fwrite(buffer, 1, count + 1, fp);
	};

	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, sizeof(buffer), "Dump Time: %d-%d-%d %d:%d:%d\n",
			curr_tm->tm_year + 1900, curr_tm->tm_mon + 1, curr_tm->tm_mday,
			curr_tm->tm_hour, curr_tm->tm_min, curr_tm->tm_sec);
	fwrite(buffer, 1, strlen(buffer), fp);

	*pfp = fp;
	return 0;
}

static void close_file(FILE *fp)
{
	if (fp)
		fclose(fp);
}

static void p_maps(FILE *fp)
{
	if (NULL == fp)
		return;
	char cmd[100] = { 0 };
	snprintf(cmd, sizeof(cmd), "cat /proc/%d/maps", getpid());
	FILE *cmd_fd = popen(cmd, "r");
	if (NULL == cmd_fd)
		return;

	char buff[128] = { 0 };
	size_t rc = 0;
	while ((rc = fread(buff, 1, sizeof(buff), cmd_fd)) > 0)
	{
		fwrite(buff, 1, rc, fp);
	}
	pclose(cmd_fd);
}

static void p_backtrace(FILE *fp)
{
	if (NULL == fp)
		return;

	int j, nptrs;
	void *buffer[512];
	char **strings;

	nptrs = backtrace(buffer, sizeof(buffer));

	printf("backtrace() returned %d addresses\n", nptrs);

	strings = backtrace_symbols(buffer, nptrs);
	if (strings == NULL) {
		perror("backtrace_symbols");
		exit(EXIT_FAILURE);
	}

	for (j = 0; j < nptrs; j++) {
		char info[128] = { 0 };
		snprintf(info, sizeof(info), "  [%02d] %s\n", j, strings[j]);
		fwrite(info, 1, strlen(info), fp);
	}

	free(strings);
}

static int dump(void)
{
	int ret = 0;
	FILE *fp = NULL;

	//open core file
	if ((ret = open_file(&fp)) != 0)
		return ret;

	//print maps
	p_maps(fp);

	//print backtrace
	p_backtrace(fp);

	//close core file
	close_file(fp);
	return 0;
}

static void sig_hdl(int signum)
{
	dump();
	signal(signum, SIG_DFL); /* 恢复信号默认处理 */
	raise(signum);           /* 重新发送信号 */
}

int dump_init(void)
{
	int n_sig = sizeof(g_sig_arr) / sizeof(g_sig_arr[0]);
	int i = 0;
	for (i = 0; i < n_sig; i++) {
		signal(g_sig_arr[i].sig_num, sig_hdl);
	}
	return 0;
}

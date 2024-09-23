#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "cmd_comm.h"


typedef struct {
	cmd_list_t base;
	char file_name[128];
} my_cmdlist_t;

static int print_help(const cmd_list_t *pcmd, const params_t *param)
{
	int cmd_idx;

	if (param && 2 == param->cmd_num) {
		for (cmd_idx = 0; cmd_idx < cmdlist_num; ++cmd_idx) {
			const cmd_list_t *elem = CMD_LIST_ELEM(cmd_idx);
			if (!strncmp(elem->name, param->cmd_params[1], PARAM_LEN))
				break;
			else {
				int id = strtoul(param->cmd_params[1], NULL, 0);
				if (0 != id && id == elem->id)
					break;
			}
		}
		if (cmd_idx < cmdlist_num) {
			const cmd_list_t *elem = CMD_LIST_ELEM(cmd_idx);
			printf("\ncommand:\n");
			printf("  %-4d:    %s\n", elem->id, elem->name);
			if (elem->help) elem->help();
			return 0;
		}
	}

	if (cmdlist_num > 0)
		printf("\ncommand list:\n");
	for (cmd_idx = 0; cmd_idx < cmdlist_num; ++cmd_idx) {
		const cmd_list_t *elem = CMD_LIST_ELEM(cmd_idx);
		printf("  %-4d:    %s\n", elem->id, elem->name);
	}

	int flag = 0;
	for (cmd_idx = 0; cmd_idx < cmdlist_num; ++cmd_idx) {
		const cmd_list_t *elem = CMD_LIST_ELEM(cmd_idx);
		if (elem->help) {
			if (0 == flag) {
				printf("command params:\n");
				flag = 1;
			}
			elem->help();
		}
	}
	return 0;
}

static int test1_fn(const cmd_list_t *pcmd, const params_t *param)
{
	printf("%s() enter.\n", __func__);
	//for (int i = 0; i < param->cmd_num; ++i)
	//	printf("%s\n", param->cmd_params[i]);

	return 0;
}

static void test1_h(void)
{
	printf("  test1 : The test command.\n");
}

static int test2_fn(const cmd_list_t *pcmd, const params_t *param)
{
	printf("%s() enter.\n", __func__);
	//for (int i = 0; i < param->cmd_num; ++i)
	//	printf("%s\n", param->cmd_params[i]);

	return 0;
}

static void test2_h(void)
{
	printf("  test2 : The test command.\n");
}

static void init_cmd(void)
{
	printf("%s() enter.\n", __func__);
}


static my_cmdlist_t mylist[] = {
	{{1,   "help",        print_help,   NULL},           ""},
	{{11,  "test1",       test1_fn,     test1_h},        ""},
	{{12,  "test2",       test2_fn,     test2_h},        ""},
};
static unsigned int num_mylist = sizeof(mylist) / sizeof(mylist[0]);
__attribute((constructor))
static void mylist_cmdlist_init(void) {
	pcmdlist    = (cmd_list_t *)mylist;
	cmdlist_num = num_mylist;
	cmdlist_size = sizeof(my_cmdlist_t);
	init_cmd();
}

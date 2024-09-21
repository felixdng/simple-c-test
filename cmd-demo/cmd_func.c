#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "cmd_comm.h"




static int test1_fn(params_t *param)
{
	printf("%s() enter.\n", __func__);
	return 0;
}

static int test2_fn(params_t *param)
{
	printf("%s() enter.\n", __func__);
	return 0;
}

static int print_help(params_t *param)
{
	int cmd_idx;
	printf("\ncommand:\n");
	for (cmd_idx = 0; cmd_idx < cmdlist_num; ++cmd_idx) {
		const cmd_list_t *elem = CMD_LIST_ELEM(cmd_idx);
		printf("\t%d:\t%s\n", elem->id, elem->name);
		if (elem->help) elem->help();
	}
	return 0;
}

static void init_cmd(void)
{
	printf("%s() enter.\n", __func__);
}

typedef struct {
	cmd_list_t base;
	char file_name[128];
} my_cmdlist_t;

static my_cmdlist_t mylist[] = {
	{{1,  "help",        print_help,   NULL}, ""},
	{{11, "test1",       test1_fn,     NULL}, ""},
	{{12, "test2",       test2_fn,     NULL}, ""},
};
static unsigned int num_mylist = sizeof(mylist) / sizeof(mylist[0]);
__attribute((constructor))
void mylist_cmdlist_init(void) {
	pcmdlist    = (cmd_list_t *)mylist;
	cmdlist_num = num_mylist;
	cmdlist_size = sizeof(my_cmdlist_t);
	init_cmd();
}

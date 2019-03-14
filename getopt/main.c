#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

enum
{
	OPT_RET_HELP     = 0x01,
	OPT_RET_DIREC    = 0x02,
	OPT_RET_L2       = 0x04,
	OPT_RET_L3       = 0x08,
	OPT_RET_L4       = 0x10,
	OPT_RET_SMAC     = 0x20,
	OPT_RET_DMAC     = 0x40,
	OPT_RET_VLAN     = 0x80,
	OPT_RET_IVLAN    = 0x100,

	OPT_RET_NUM
} E_OPT_RET;
static void showusage(void)
{
	printf("Usage : sock [options]\n");
	printf("Options:\n");
	printf(" -h, --help                              Print this message.\n");
	printf(" -D, --direction                         IO direction: read or write\n");
	printf(" --l2 [l2type],                           Protocol level 2, eg:mac,vlan...\n");
	printf("     l2type:\n");
	printf("       mac : --smac [smac] | --dmac [dmac]\n");
	printf("       vlan: --vlan [id] --ivlan [id]\n");
	printf(" --l3,                                    Protocol level 3, eg:ip...\n");
	printf(" --l4,                                    Protocol level 4, eg:tcp,udp...\n");
	printf("");
}

static int opt_ret;
static struct option longOpts[] = {
	{"help",      no_argument,       &opt_ret, OPT_RET_HELP},
	{"direction", required_argument, &opt_ret, OPT_RET_DIREC},
	{"l2",        required_argument, &opt_ret, OPT_RET_L2},
	{"l3",        required_argument, &opt_ret, OPT_RET_L3},
	{"l4",        required_argument, &opt_ret, OPT_RET_L4},
	{"smac",      required_argument, &opt_ret, OPT_RET_SMAC},
	{"dmac",      required_argument, &opt_ret, OPT_RET_DMAC},
	{"vlan",      required_argument, &opt_ret, OPT_RET_VLAN},
	{"ivlan",     required_argument, &opt_ret, OPT_RET_IVLAN},
	{0, 0, 0, 0},
};
static int parse_parameter(int argc, char **argv)
{
	while (1)
	{
		int ret = getopt_long(argc, argv, "hD:", longOpts, NULL);
		if (-1 == ret)
			break;
	}
	return 0;
}

int main(int argc, char **argv)
{
	parse_parameter(argc, argv);
	return 0;
}


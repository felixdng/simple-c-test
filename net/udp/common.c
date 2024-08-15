#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int get_params(int argc, char *argv[], char **ipaddr, int *port)
{
	if (argc != 3)
		goto err;
	if (0 == argv[1][0] || 0 == argv[2][0])
		goto err;
	int pt = strtoul(argv[2], NULL, 0);
	if (pt < 1024 || pt > 0xffff) {
		printf("error port(%s)\n", argv[2]);
		goto err;
	}

	if (ipaddr)
		*ipaddr = argv[1];
	if (port)
		*port = pt;
	return 0;

err:
	printf("Usage: %s <ip_addr> <port>\n", argv[0]);
	return -1;
}

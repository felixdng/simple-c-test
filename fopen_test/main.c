#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int execl_cmd(const char *command, const char *type, char *output)
{
	char buf[1024];
	FILE *fp;

	sprintf(buf, "%s", command);
	fp = popen(buf, type);
	if (fp == NULL) {
		printf("popen() error!/n");
		return -1;
	}

	while (fgets(buf, sizeof(buf), fp)) {
		if (output != NULL)
			strcat(output,buf);
	}

	pclose(fp);
	return 0;
}

int main(int argc, char **argv)
{
	char cmd_str[64];
	char rest_str[1024];
	char type[2];

	if (argc != 3) {
		printf("usage: %s [shell cmd] [type]\n", argv[0]);
		printf("ex: %s \"ls /home/\" \"r\" \n", argv[0]);
		exit(1);
	}

	strcpy(cmd_str, argv[1]);
	type[0] = argv[2][0];
	type[1] = '\0';
	memset(rest_str,0,sizeof(rest_str));
	execl_cmd(cmd_str, type, rest_str);
	printf("result : %s\n",rest_str);

	return 0;
}

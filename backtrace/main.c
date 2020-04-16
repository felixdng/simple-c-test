#include <stdio.h>

extern int dump_init(void);
int main(int argc, char *argv[])
{
	dump_init();

	//test for dump
	do {
		int *ptr = NULL;
		*ptr = 123;
	} while (0);
	return 0;
}

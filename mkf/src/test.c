#include <stdio.h>

int test_func(void)
{
	printf("%s running.\n", __func__);
	return 0;
}

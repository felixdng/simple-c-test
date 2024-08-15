#include <stdio.h>
#include "base.h"

int main(void)
{
#if defined(TEST)
	test_func();
#else
	printf("main running\n");
#endif

	double x = 8.0;
	printf("sqrt(%f)=%f\n", x, mysqrt(x));
	return 0;
}

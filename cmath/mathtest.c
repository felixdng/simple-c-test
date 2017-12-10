#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mathbase.h"


int main(int argc, char **argv)
{
	int_8 num1[] = "2345743445";
	int_8 num2[] = "7235342232383";
	int_8 *ret = NULL;
	ret = plus(num1, num2);
	if (ret) {
		printf("%s + %s = %s\n", num1, num2, ret);
		free(ret);
	}

	ret = minus(num2, num1);
	if (ret) {
		printf("%s - %s = %s\n", num2, num1, ret);
		free(ret);
	}
	return 0;
}


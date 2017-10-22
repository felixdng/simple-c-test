#include <stdio.h>
#include "print.h"

int main(int argc, char **argv)
{
	USER_PRINT(USER_DEBUG,"hello-debug");
	USER_PRINT(USER_INFO,"hello-info : %s %d", "num", 4);
	USER_PRINT(USER_ERR,"hello-err");
	return 0;
}

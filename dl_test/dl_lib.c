#include <stdio.h>

#define _init(x) \
__attribute__((constructor(101+(x))))

#define _exit \
__attribute__((destructor))

_init(2) void func2(void)
{
	printf("%s()\n", __func__);
}

_init(3) void func3(void)
{
	printf("%s()\n", __func__);
}

_init(1) void func1(void)
{
	printf("%s()\n", __func__);
}

_exit void func_exit(void)
{
	printf("%s()\n", __func__);
}

void test_main(void)
{
	printf("%s()\n", __func__);
}


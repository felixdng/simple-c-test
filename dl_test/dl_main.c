#include <stdio.h>
#include <dlfcn.h>

typedef void (*fn)(void);
int main(int argc, char *argv[])
{
	int cnt = 0;

	while (cnt < 2) {
	void *call = dlopen("libdltest.so", RTLD_NOW|RTLD_GLOBAL);
	if (NULL == call)
		return -1;

	fn m = dlsym(call, "test_main");
	if (m) m();

	dlclose(call);

	getchar();
	cnt++;
	}

	return 0;
}


#include <stdio.h>
#include <string.h>
#include <unistd.h>

void copy_data(int from, int to)
{
	char buf[1024];
	int amount;
	while ((amount = read(from, buf, sizeof(buf))) > 0) {
		write(to, buf, amount);
		if (0 == strncmp(buf, "exit", 4))
			break;
	}
}


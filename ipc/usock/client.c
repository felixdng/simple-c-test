#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "common.h"

int main(int argc, char *argv[])
{
	struct sockaddr_un address;
	int sock;
	size_t addrlen;

	if ((sock = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("socket failed!");
		return -1;
	}

	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, "./sample-socket");
	addrlen = sizeof(address.sun_family) + strlen(address.sun_path);

	connect(sock, (struct sockaddr *)&address, addrlen);

	copy_data(0, sock);
	close(sock);
	return 0;
}


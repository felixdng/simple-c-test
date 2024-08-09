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
	int sock, conn;
	socklen_t addrlen;

	if ((sock = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
		perror("socket failed!");
		return -1;
	}

	unlink("./sample-socket");

	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, "./sample-socket");
	addrlen = sizeof(address.sun_family) + strlen(address.sun_path);

	if (bind(sock, (struct sockaddr *)&address, addrlen)) {
		perror("bind failed!");
		close(sock);
		return -1;
	}

	if (listen(sock, 5)) {
		perror("listen failed!");
		close(sock);
		return -1;
	}

	conn = accept(sock, (struct sockaddr *)&address, &addrlen);
	copy_data(conn, 1);
	close(conn);

	close(sock);
	return 0;
}

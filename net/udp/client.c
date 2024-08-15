#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "common.h"


static int udp_client(const char *ip, int port)
{
	int sock_fd;
	struct sockaddr_in sin;
	char buf[BUFSIZ];
	int ret = 0;

	//创建套接字
	if ( (sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		return -1;
	}

	//地址复用
    int opt = 1;
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt))) {
		perror("setsockopt failed");
		goto err;
	} 

	//填充地址
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);  
	sin.sin_addr.s_addr = inet_addr(ip);

	//与服务端进行通信
	while(1) {
		bzero(buf, BUFSIZ);
		if (NULL == fgets(buf, BUFSIZ, stdin))
			break;
		if(strncmp(buf, "quit", 4) == 0)
			break;
		sendto(sock_fd, buf, strlen(buf) + 1, 0, (struct sockaddr*)&(sin), sizeof(sin));
		ret = recvfrom(sock_fd, buf, BUFSIZ, 0, NULL, 0);
		if (ret <= 0) break;
		printf("recv:%s\n", buf);
	}

	close(sock_fd);
	return 0;

err:
	close(sock_fd);
	return -1;
}

int main(int argc, char *argv[])
{
	char *ip = NULL;
	int port = 0;

	if (0 != get_params(argc, argv, &ip, &port))
		exit(1);

	udp_client(ip, port);
	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	if (argc != 3) {
		printf("Usage: %s <srv_ip> <srv_port>\n", argv[0]);
		exit(1);
	}

	char *srv_ip = argv[1];
	int srv_port = strtoul(argv[2], NULL, 0);

	int sock_fd;
	struct sockaddr_in sin;
	char buf[BUFSIZ];
	int rc_siz;

	//创建套接字
	if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket create");
		exit(1);
	}

	//填充地址
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(srv_port);
	sin.sin_addr.s_addr = inet_addr(srv_ip);

	//申请连接服务器
	if (connect(sock_fd, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		perror("connect to service");
		exit(1);
	}

	//与服务端进行通信
	while (1) {
		snprintf(buf, sizeof(buf), "what's the number?\r");
		send(sock_fd, buf, strlen(buf), 0);

		bzero(buf, sizeof(buf));
		rc_siz = recv(sock_fd, buf, sizeof(buf), 0);
		if (rc_siz <= 0)
			break;
		printf("recv: %u\n", rc_siz);
		printf("  %s\n", buf);

		snprintf(buf, sizeof(buf), "exit\r");
		send(sock_fd, buf, strlen(buf), 0);
		break;
	}

	close(sock_fd);
	return 0;
}

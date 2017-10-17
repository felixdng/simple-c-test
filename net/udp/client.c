#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#define PORT 5678
#define SRV_IP "192.168.56.10"

int udp_client(void)
{
	int sock_fd;
	struct sockaddr_in sin;
	char buf[BUFSIZ];

	//创建套接字
	if ( (sock_fd = socket(AF_INET, SOCK_DGRAM,0)) < 0) {
		perror("socket");
		exit(1);
	}

	//填充地址
	bzero(&sin,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);  
	sin.sin_addr.s_addr = inet_addr(SRV_IP);

	//与服务端进行通信
	while(1){
		bzero(buf, BUFSIZ);
		fgets(buf, BUFSIZ, stdin);
		sendto(sock_fd, buf, strlen(buf)+1, 0, (struct sockaddr*)&(sin), sizeof(sin));
		if(strncmp(buf,"quit",4) == 0)
			break;
	}

	close(sock_fd);
	return 0;
}

int main(int argc, char *argv[])
{
	udp_client();
	return 0;
}

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
	char *pip = argv[1];
	int port = strtoul(argv[2], NULL, 0);
	int sock_fd;
	int conn_fd;
	struct sockaddr_in sin,cin;
	char buf[BUFSIZ];
	//int ret;

	//创建套接字
	if( (sock_fd = socket(AF_INET,SOCK_STREAM,0)) < 0){
		printf("socket fail\n");
		return -1;
	}

	//填充地址
	bzero(&sin,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	sin.sin_addr.s_addr = inet_addr(pip);

	int on = 1;
	if((setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))) < 0)
	{
		printf("setsockopt failed\n");
		return -1;
	}

	//绑定地址和端口
	if(bind(sock_fd,(struct sockaddr*)&sin,sizeof(sin)) < 0){
		printf("bind fail\n");
		return -1;
	}
	printf("start service : %s %u\n", pip, port);

	//监听
	if(listen(sock_fd, 8) < 0){
		printf( "listen fail\n");
		return -1;
	}
	socklen_t len = sizeof(cin);    //通过cin获得客户端的IP
	printf( "wait client...\n");
	if( (conn_fd = accept(sock_fd,(struct sockaddr*)&cin,&len)) < 0){ //没有客户端，进程阻塞
		printf("accept faile\n");
		return -1;
	}

	sprintf(buf, "ifconfig\r");
	send(conn_fd, buf, strlen(buf), 0);
	
	bzero(buf, sizeof(buf));
	int rn = read(conn_fd, buf, BUFSIZ);
	printf("recv from client [%u]:\n", rn);
	printf("%s\n", buf);

	close(conn_fd);
	close(sock_fd);

	return 0;
}

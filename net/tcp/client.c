#include "net.h"
int main(void)
{

	int sock_fd;
	struct sockaddr_in sin;
	char buf[BUFSIZ];
	int ret;

	//创建套接字
	if( (sock_fd = socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("socket");
		exit(1);
	}

	//填充地址
	bzero(&sin,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);  
	sin.sin_addr.s_addr = inet_addr(SRV_IP);


	//申请连接服务器
	if( connect(sock_fd,(struct sockaddr*)&sin,sizeof(sin)) < 0){ //没有客户端，进程阻塞
		perror("connect");
		exit(1);
	}

	//与客户读进行通信
	while(1){
		fgets(buf,BUFSIZ,stdin);
		write(sock_fd,buf,strlen(buf));
		if(strncmp(buf,"quit",4) == 0)
			break;
	}


	close(sock_fd);
	return 0;
}

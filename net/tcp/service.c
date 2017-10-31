#include "net.h"
int main(void)
{

	int sock_fd,conn_fd;
	struct sockaddr_in sin,cin;
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

	//绑定地址和端口
	if(bind(sock_fd,(struct sockaddr*)&sin,sizeof(sin)) < 0){
		perror("bind");
		exit(1);
	}

	//监听
	if(listen(sock_fd,N) < 0){
		perror("listen");
		exit(1);
	}

	//等待客户端的连接
	printf("等待连接中....\n");
	socklen_t len = sizeof(cin);	//通过cin获得客户端的IP
	if( (conn_fd = accept(sock_fd,(struct sockaddr*)&cin,&len)) < 0){ //没有客户端，进程阻塞
		perror("accept");
		exit(1);
	}
	//打印客户端IP
	printf("%s",inet_ntoa(cin.sin_addr));
	//与客户读进行通信
	while(1){
		bzero(buf,BUFSIZ);
		if( (ret = read(conn_fd,buf,BUFSIZ)) < 0){
			perror("read");
			exit(1);
		}else if(ret == 0){
			printf("断开连接\n");
			break;
		}
		if(strncmp(buf,"quit",4) == 0)
			break;
		printf("%s",buf);
	}
	close(sock_fd);
	close(conn_fd);
	return 0;
}

#include "net.h"
#include <signal.h>
#include <sys/wait.h>


void sig_chld(int signo)
{
	pid_t pid;
	int   stat;
	pid = wait(&stat);
	printf("child %d terminated\n", pid);
	return;
}

int main(void)
{
	int sock_fd,conn_fd;
	struct sockaddr_in sin,cin;
	char buf[BUFSIZ];
	int ret;

	signal(SIGCHLD, sig_chld);

	if( (sock_fd = socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("socket");
		exit(1);
	}

	bzero(&sin,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	sin.sin_addr.s_addr = inet_addr(SRV_IP);

	if(bind(sock_fd,(struct sockaddr*)&sin,sizeof(sin)) < 0){
		perror("bind");
		exit(1);
	}

	if(listen(sock_fd,N) < 0){
		perror("listen");
		exit(1);
	}

	while (1) {
		socklen_t len = sizeof(cin);
		if( (conn_fd = accept(sock_fd,(struct sockaddr*)&cin,&len)) < 0){
			perror("accept");
			exit(1);
		}

		pid_t pid = fork();
		if (pid < 0) {
			perror("fork");
			exit(1);
		} else if (0 == pid) {
			//child
			close(sock_fd);

			printf("%s\n",inet_ntoa(cin.sin_addr));
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
				printf("%s\n",buf);
				write(conn_fd,buf,strlen(buf)+1);
			}
			close(conn_fd);
			exit(0);
		} else {
			//parent
			close(conn_fd);
		}
	}
	close(sock_fd);
	return 0;
}

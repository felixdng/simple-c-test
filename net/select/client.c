#include "net.h"
#include <sys/select.h>


int main(void)
{
	int sock_fd;
	struct sockaddr_in sin;
	char buf[BUFSIZ];
	int ret;
	fd_set rset;
	int maxfd = 0;

	if( (sock_fd = socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("socket");
		exit(1);
	}

	bzero(&sin,sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);
	sin.sin_addr.s_addr = inet_addr(SRV_IP);

	if( connect(sock_fd,(struct sockaddr*)&sin,sizeof(sin)) < 0){
		perror("connect");
		exit(1);
	}
	printf("connect to %s:%d.\n", inet_ntoa(sin.sin_addr), ntohs(sin.sin_port));

	maxfd = 0;
	FD_ZERO(&rset);
	FD_SET(sock_fd, &rset);
	maxfd = sock_fd > maxfd ? sock_fd : maxfd;
	FD_SET(fileno(stdin), &rset);
	maxfd = fileno(stdin) > maxfd ? fileno(stdin) : maxfd;
	while(1) {
		fd_set _rset = rset;
		select(maxfd+1, &_rset, NULL, NULL, NULL);
		if (FD_ISSET(sock_fd, &_rset)) {
			ret = read(sock_fd, buf, sizeof(buf));
			if (ret <= 0) {
				printf("ret=%d\n", ret);
				break;
			}
			printf("rcv:%s\n", buf);
		}

		if (FD_ISSET(fileno(stdin), &_rset)) {
			printf("input> ");
			fflush(stdout);
			fgets(buf,BUFSIZ,stdin);
			write(sock_fd,buf,strlen(buf));
			if(strncmp(buf,"quit",4) == 0)
				break;
		}
	}

	close(sock_fd);
	return 0;
}

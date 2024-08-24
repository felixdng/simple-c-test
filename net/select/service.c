#include "net.h"
#include <sys/select.h>


typedef struct {
	int fd;
	struct sockaddr_in sin;
} addrinfo_t;
int main(int argc, char *argv[])
{
	int sock_fd;
	struct sockaddr_in sin;
	if( (sock_fd = socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("socket");
		exit(1);
	}
	int opt = 1;
	if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&opt, sizeof(opt))) {
		perror("setsockopt failed");
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

	fd_set rset, _rset;
	FD_ZERO(&rset);
	FD_SET(sock_fd, &rset);
	int ic;
	addrinfo_t clis[N] = {};
	for (ic = 0; ic < N; ++ic) {
		clis[ic].fd = -1;
	}
	int maxfd = sock_fd;

	while (1) {
		_rset = rset;
		int nfds = select(maxfd + 1, &_rset, NULL, NULL, NULL);
		if (-1 == nfds)
			exit(1);

		// client connect
		if (FD_ISSET(sock_fd, &_rset)) {
			struct sockaddr_in cin = {};
			socklen_t len = sizeof(cin);
			int conn_fd = -1;
			if( (conn_fd = accept(sock_fd, (struct sockaddr *)&cin, &len)) < 0) {
				perror("accept");
				exit(1);
			}
			printf("%s:%d connect.\n", inet_ntoa(cin.sin_addr), ntohs(cin.sin_port));
			for (ic = 0; ic < N; ++ic) {
				if (-1 == clis[ic].fd)
					break;
			}
			if (ic >= N) {
				printf("client number over max!\n");
				close(conn_fd);
			} else {
				clis[ic].fd = conn_fd;
				clis[ic].sin = cin;
				FD_SET(conn_fd, &rset);
				if (conn_fd > maxfd) {
					maxfd = conn_fd;
				}
			}

			if (--nfds <= 0) continue;
		}

		for (ic = 0; ic < N && nfds > 0; ++ic) {
			if (-1 == clis[ic].fd)
				continue;
			if (!FD_ISSET(clis[ic].fd, &_rset))
				continue;
			--nfds;
			char buf[BUFSIZ];
			bzero(buf, BUFSIZ);
			int rc;
			if( (rc = read(clis[ic].fd,buf,BUFSIZ)) < 0){
				perror("read");
				exit(1);
			} else if ((rc == 0) || (rc > 0 && (strncmp(buf,"quit",4) == 0))) {
				printf("%s:%d disconnect.\n", inet_ntoa(clis[ic].sin.sin_addr), ntohs(clis[ic].sin.sin_port));
				if (rc > 0 && (strncmp(buf,"quit",4) == 0))
					close(clis[ic].fd);
				FD_CLR(clis[ic].fd, &rset);
				clis[ic].fd = -1;
				continue;
			}
			printf("[from %s:%d]%s\n", inet_ntoa(clis[ic].sin.sin_addr), ntohs(clis[ic].sin.sin_port) ,buf);
			write(clis[ic].fd, buf, rc);
		}
	}

	close(sock_fd);
	return 0;
}

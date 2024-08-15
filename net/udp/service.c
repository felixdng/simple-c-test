#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "common.h"

#if 0
#include <sys/ioctl.h>
#include <netdb.h>  
#include <net/if.h>
#include <errno.h> 
#endif

//#define PORT		5678
//#define SRV_IP		"192.168.56.10"

#if 0
#define IP_SIZE		16

// 获取本机ip  
int get_local_ip(const char *eth_inf, char *ip)  
{  
	int sd;  
	struct sockaddr_in sin;  
	struct ifreq ifr;  

	sd = socket(AF_INET, SOCK_DGRAM, 0);  
	if (-1 == sd)  
	{  
		printf("socket error: %s\n", strerror(errno));  
		return -1;        
	}  

	strncpy(ifr.ifr_name, eth_inf, IFNAMSIZ);  
	ifr.ifr_name[IFNAMSIZ - 1] = 0;  

	// if error: No such device  
	if (ioctl(sd, SIOCGIFADDR, &ifr) < 0)  
	{  
		printf("ioctl error: %s\n", strerror(errno));  
		close(sd);  
		return -1;  
	}  

	memcpy(&sin, &ifr.ifr_addr, sizeof(sin));  
	snprintf(ip, IP_SIZE, "%s", inet_ntoa(sin.sin_addr));  

	close(sd);  
	return 0;  
}
#endif

int udp_service(const char *ip, int port)
{
	int sock_fd;
	struct sockaddr_in sin;
	char wbuf[BUFSIZ + 3];
	char rbuf[BUFSIZ];
	int ret;

	//创建套接字
	if ( (sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket failed");
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

	//绑定地址和端口
	if (bind(sock_fd, (struct sockaddr*)&sin, sizeof(sin)) < 0){
		perror("bind");
		goto err;
	}

	//与客户读进行通信
	printf("等待客户端...\n");
	while (1) {
		bzero(rbuf, sizeof(rbuf));
		struct sockaddr_in cli = {};
		socklen_t clen = sizeof(cli);
		if ( (ret = recvfrom(sock_fd, rbuf, sizeof(rbuf), 0, (struct sockaddr*)&cli, &clen)) < 0) {
			perror("read");
			goto err;
		} else if(ret == 0) {
			printf("断开连接\n");
			//break;
			sleep(3);
			continue;
		}

		printf("recv: %s\n", rbuf);
		snprintf(wbuf, sizeof(wbuf), "RC:%s", rbuf);
		sendto(sock_fd, wbuf, strlen(wbuf) + 1, 0, (struct sockaddr*)&(cli), sizeof(cli));
		//if(strncmp(buf,"quit",4) == 0)
		//	break;
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

	udp_service(ip, port);
	return 0;
}

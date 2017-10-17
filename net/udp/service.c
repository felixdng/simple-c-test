#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#if 0
#include <sys/ioctl.h>
#include <netdb.h>  
#include <net/if.h>
#include <errno.h> 
#endif

#define PORT		5678
#define SRV_IP		"192.168.56.10"

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

int udp_service(void)
{
	int sock_fd;
	struct sockaddr_in sin;
	char buf[BUFSIZ];
	int ret;

	//创建套接字
	if ( (sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket failed");
		return -1;
	}

	//填充地址
	bzero(&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORT);  
	sin.sin_addr.s_addr = inet_addr(SRV_IP);

	//绑定地址和端口
	if (bind(sock_fd, (struct sockaddr*)&sin, sizeof(sin)) < 0){
		perror("bind");
		return -1;
	}

	//与客户读进行通信
	printf("等待客户端...\n");
	while (1) {
		bzero(buf,BUFSIZ);
		if ( (ret = recvfrom(sock_fd, buf, BUFSIZ, 0, NULL, 0)) < 0) {
			perror("read");
			return -1;
		}else if(ret == 0){
			printf("断开连接\n");
			break;
		}
		if(strncmp(buf,"quit",4) == 0)
		  break;
		printf("%s",buf);
	}

	close(sock_fd);
	return 0;
}

int main(int argc, char *argv[])
{
	udp_service();
	return 0;
}

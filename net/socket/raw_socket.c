#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>

static int init_raw_socket(int *raw_sockfd, const char *if_name)
{
	int fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (fd < 0)
	{
		fprintf(stderr, "%s():[%d] socket failed!\n", __func__, __LINE__);
		return -1;
	}

	/* bind to interface */
	struct ifreq req;
	memset(&req, 0, sizeof(req));
	strcpy((char*)req.ifr_name, (char*)if_name);

	if (ioctl(fd, SIOCGIFINDEX, &req) < 0)
	{
		fprintf(stderr, "%s():[%d] ioctl faild!\n", __func__, __LINE__);
		close(fd);
		return -1;
	}

	struct sockaddr_ll addr;
	memset(&addr, 0, sizeof(addr));
	addr.sll_family   = PF_PACKET;
	addr.sll_protocol = 0;
	addr.sll_ifindex  = req.ifr_ifindex;
	printf("ifindex: %x\n", req.ifr_ifindex);

	if (bind(fd, (const struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		fprintf(stderr, "%s():[%d] bind failed!\n", __func__, __LINE__);
		close(fd);
		return -1;
	}
#if 1
	if ( ioctl( fd, SIOCGIFHWADDR, &req ) < 0 )
	{
		perror( "init: ioctl SIOCGIFHWADDR" );
		close( fd );
		return -1;
	}
	printf("%s mac", if_name);
	int i;
	for (i = 0; i < 6; ++i)
		printf(":%02x", (unsigned char)req.ifr_hwaddr.sa_data[i]);
	printf("\n");
#endif
	*raw_sockfd = fd;
	return 0;
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("usg: %s <device>\n", argv[0]);
		exit(1);
	}
	char* ifname = argv[1];
	int raw_sock = -1;
loop:
	if (init_raw_socket(&raw_sock, ifname) != 0)
	{
		fprintf(stderr, "%s():[%d] init_raw_socket failed!\n", __func__, __LINE__);
		sleep(5);
		goto loop;
	}

	int flag = 3;
	while (flag > 0)
	{
	char buf[BUFSIZ];
	int bufLen = 0;
	bzero(buf, sizeof(buf));
	bufLen = read(raw_sock, buf, sizeof(buf));
	if (bufLen < 0)
	{
		printf("buffer length less 0\n");
		break;
	}
	else if (bufLen > 0)
	{
		int i;
		printf("\nbufLen: %d\n", bufLen);
		printf("buf: ");
		for (i = 0; i < bufLen; ++i)
			printf("%02x ", (unsigned char)buf[i]);
		printf("\n\n");
		sleep(1);
	}
	else
	{
		printf("buffer length =0\n");
		sleep(1);
		continue;
	}
	}

	close(raw_sock);
	return 0;
}

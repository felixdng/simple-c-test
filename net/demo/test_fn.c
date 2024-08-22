#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>	

#include "unp.h"

typedef union {
	uint64_t _u64;
	uint8_t _u8[8];
} byte8_t;

uint64_t _hton64(uint64_t x)
{
#if (__BYTE_ORDER__==__ORDER_LITTLE_ENDIAN__)
	uint32_t l = x & 0xffffffff;
	uint32_t h = x >> 32 & 0xffffffff;
	l = htonl(l);
	h = htonl(h);
	x = (uint64_t)l << 32 | h;
#endif
	return x;
}

uint64_t _ntoh64(uint64_t x)
{
#if (__BYTE_ORDER__==__ORDER_LITTLE_ENDIAN__)
	uint32_t l = x & 0xffffffff;
	uint32_t h = x >> 32 & 0xffffffff;
	l = ntohl(l);
	h = ntohl(h);
	x = (uint64_t)l << 32 | h;
#endif
	return x;
}

static void pr_byte(uint64_t data)
{
	int i = 0;
#if 0
	char str[17] = {};
	uint64_t tmp = data;
	while (tmp /= 0x100) i++;
	tmp = data;
	do {
		str[i * 2]     = (tmp % 0x100) / 0x10 + '0';
		str[i * 2 + 1] = (tmp % 0x100) % 0x10 + '0';
		i--;
		tmp /= 0x100;
	} while (tmp);
	printf("%s\n", str);
#else
	byte8_t bytes = {};
	bytes._u64 = data;
	for (i = 0; i < 8; ++i)
		printf("%d#%02x ", i, bytes._u8[i]);
	printf("\n");
#endif
}

static void host_net(void)
{
	uint16_t hs = 0x1234;
	uint32_t hl = 0x12345678;
	uint64_t hll = 0x1234567890876543;

	uint16_t ns = htons(hs);
	uint32_t nl = htonl(hl);
	uint64_t nll = _hton64(hll);

	printf("\n---- host to net ----\n");
	printf("hs=%x\n", hs);
	printf("hs: ");
	pr_byte(hs);
	printf("ns: ");
	pr_byte(ns);

	printf("hl=%x\n", hl);
	printf("hl: ");
	pr_byte(hl);
	printf("nl: ");
	pr_byte(nl);

	printf("hll:%lx\n", hll);
	printf("hll: ");
	pr_byte(hll);
	printf("nll: ");
	pr_byte(nll);
}

static void string(void)
{
	printf("\n---- string ----\n");
	char pt[] = "hello";
	char str1[16];
	char str2[16];

	bzero(str1, sizeof(str1));
	bcopy(pt, str1, sizeof(pt));
	printf("str1:%s\n", str1);
	printf("bcmp:%d\n", bcmp(str1, str2, sizeof(str1)));

	memset(str2, 0, sizeof(str2));
	memcpy(str2, pt, sizeof(pt));
	printf("str2:%s\n", str2);
	printf("memcmp:%d\n", memcmp(str1, str2, sizeof(str1)));
}

static void addr(void)
{
	printf("\n---- address ----\n");

	struct in_addr i4 = {};
	char a4[INET_ADDRSTRLEN] = "192.168.100.123";
	if (inet_aton(a4, &i4))
		printf("inet_aton: [%s]:(%08x)\n", a4, i4.s_addr);
	else
		printf("inet_aton: %s: not ipv4\n", a4);

	char *pa = inet_ntoa(i4);
	if (pa)
		printf("inet_ntoa: (%08x):[%s]\n", i4.s_addr, pa);

	if (inet_pton(AF_INET, "172.168.200.100", &i4))
		printf("inet_pton: %08x\n", i4.s_addr);
	else
		printf("inet_pton: failed!\n");

	struct in6_addr i6 = {};
	if (inet_pton(AF_INET6, "f111:e222:d333:c444:b555:a666:7a77:8b88", &i6))
		printf("inet_pton: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n", 
			i6.s6_addr[0], i6.s6_addr[1], i6.s6_addr[2], i6.s6_addr[3],
			i6.s6_addr[4], i6.s6_addr[5], i6.s6_addr[6], i6.s6_addr[7],
			i6.s6_addr[8], i6.s6_addr[9], i6.s6_addr[10], i6.s6_addr[11],
			i6.s6_addr[12], i6.s6_addr[13], i6.s6_addr[14], i6.s6_addr[15]);
	else
		printf("inet_pton: failed!\n");

	if (inet_ntop(AF_INET, &i4, a4, sizeof(a4)))
		printf("inet_ntop: %s\n", a4);
	else
		printf("inet_ntop:failed!\n");

	char a6[INET6_ADDRSTRLEN] = {};
	if (inet_ntop(AF_INET6, &i6, a6, sizeof(a6)))
		printf("inet_ntop: %s\n", a6);
	else
		printf("inet_ntop:failed!\n");


	struct sockaddr_in sin = {
		.sin_family = AF_INET,
		.sin_addr = i4,
		.sin_port = htons(5678)
	};
	const char *ptr = sock_ntop((struct sockaddr *)&sin, sizeof(sin));
	if (ptr) printf("%s\n", ptr);

	struct sockaddr_in6 sin6 = {
		.sin6_family = AF_INET6,
		.sin6_addr = i6,
		.sin6_port = htons(12345)
	};
	ptr = sock_ntop((struct sockaddr *)&sin6, sizeof(sin6));
	if (ptr) printf("%s\n", ptr);

}

int test_fn(int argc, char *argv[])
{
	host_net();
	string();
	addr();

	return 0;
}


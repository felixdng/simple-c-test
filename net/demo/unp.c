#include <stdio.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>	
#include <unistd.h>
#include <errno.h>


const char *_ntop4(struct sockaddr_in *sin, char *str, uint32_t len)
{
	char addr_s[INET_ADDRSTRLEN] = {};
	if (NULL == inet_ntop(AF_INET, &sin->sin_addr, addr_s, sizeof(addr_s)))
		return NULL;

	char port_s[8] = {};
	if (0 != ntohs(sin->sin_port)) {
		snprintf(port_s, sizeof(port_s), ":%d", ntohs(sin->sin_port));
	}

	memset(str, 0, len);
	snprintf(str, len, "%s%s", addr_s, port_s);
	return str;
}

const char *_ntop6(struct sockaddr_in6 *sin6, char *str, uint32_t len)
{
	char addr_s[INET6_ADDRSTRLEN] = {};
	if (NULL == inet_ntop(AF_INET6, &sin6->sin6_addr, addr_s, sizeof(addr_s)))
		return NULL;

	char port_s[8] = {};
	if (0 != ntohs(sin6->sin6_port)) {
		snprintf(port_s, sizeof(port_s), ":%d", ntohs(sin6->sin6_port));
	}

	memset(str, 0, len);
	snprintf(str, len, "[%s]%s", addr_s, port_s);
	return str;
}

const char *sock_ntop(const struct sockaddr *sa, socklen_t salen)
{
	static char s_str[128];
	const char *p_str = NULL;
	switch (sa->sa_family) {
	case AF_INET:
	{
		struct sockaddr_in *sin = (struct sockaddr_in *)sa;
		p_str = _ntop4(sin, s_str, sizeof(s_str));
	}
		break;
	case AF_INET6:
	{
		struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)sa;
		p_str = _ntop6(sin6, s_str, sizeof(s_str));
	}
		break;
	default:
		break;
	}
	return p_str;
}

ssize_t readn(int fd, void *buf, size_t nb)
{
	void *ptr = buf;
	size_t nleft = nb;

	while (nleft > 0) {
		ssize_t nread = read(fd, ptr, nleft);
		if (nread < 0) {
			if (EINTR == errno)
				nread = 0;
			else
				return -1;
		} else if (0 == nread)
			break;

		nleft -= nread;
		ptr += nread;
	}
	return nb - nleft;
}

ssize_t writen(int fd, const void *buf, size_t nb)
{
	const void *ptr = buf;
	size_t nleft = nb;

	while (nleft > 0) {
		ssize_t nwrite = write(fd, ptr, nleft);
		if (nwrite <= 0) {
			if (nwrite < 0 && EINTR == errno)
				nwrite = 0;
			else
				return -1;
		}
		nleft -= nwrite;
		ptr += nwrite;
	}
	return nb;
}

ssize_t readline(int fd, void *buf, size_t maxlen)
{
	char *ptr = buf;
	size_t nr;
	for (nr = 0; nr < (maxlen - 1); ++nr) {
		char ch;
		size_t rc;
again:
		if ((rc = read(fd, &ch, 1)) >= 1) {
			ptr[nr] = ch;
			if ('\n' == ch) {
				nr++;
				break;
			}
		} else if (0 == rc) {
			break;
		} else {
			if (EINTR == errno)
				goto again;
			return -1;
		}
	}
	ptr[nr] = '\0';
	return nr;
}


#ifndef __UNP_H
#define __UNP_H

const char *sock_ntop(const struct sockaddr *sa, socklen_t addrlen);
ssize_t readn(int fd, void *buf, size_t nb);
ssize_t writen(int fd, void *buf, size_t nb);
ssize_t readline(int fd, void *buf, size_t maxlen);

#endif /* __UNP_H */

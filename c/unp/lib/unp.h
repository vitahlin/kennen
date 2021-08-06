#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int wrapSocket(int domain, int type, int protocol);
void wrapClose(int sock_fd);
void wrapBind(int sock_fd, const struct sockaddr *address, socklen_t socklen);

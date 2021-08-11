#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/errno.h>

typedef void SignalFunc(int);

// socket相关函数
int wrapSocket(int domain, int type, int protocol);

void wrapClose(int sock_fd);

void wrapBind(int sock_fd, const struct sockaddr *address, socklen_t socklen);

// 信号相关函数
SignalFunc *wrapSignal(int sig_no, SignalFunc *func);

ssize_t wrapReadn(int fd, void *buff, size_t n);

ssize_t wrapWriten(int fd, const void *vptr, size_t n);

ssize_t wrapReadlineV1(int fd, void *vptr, size_t max_len);

ssize_t wrapReadlineV2(int fd, void *vptr, size_t max_len);


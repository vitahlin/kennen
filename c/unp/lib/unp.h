#include <sys/socket.h>
#include <unistd.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <strings.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include "./constant.h"

typedef void SignalFunc(int);

// socket相关函数
int wrapSocket(int domain, int type, int protocol);

void wrapClose(int sock_fd);

void wrapBind(int sock_fd, const struct sockaddr *address, socklen_t socklen);

void wrapListen(int sock_fd, int listen_num);

int wrapAccept(int sock_fd, struct sockaddr *cli_address, socklen_t *cli_address_length);

void wrapInetPton(int domain, const char *ip, void *dst);

void wrapConnect(int sock_fd, const struct sockaddr *serv_addr, socklen_t addr_length);

// 信号相关函数
SignalFunc *wrapSignal(int sig_no, SignalFunc *func);

/**
 * 读取多个字节
 * @param fd
 * @param buff
 * @param n
 * @return
 */
ssize_t wrapReadn(int fd, void *buff, size_t n);

ssize_t wrapWriten(int fd, const void *vptr, size_t n);

ssize_t wrapReadlineV1(int fd, void *vptr, size_t max_len);

ssize_t wrapReadlineV2(int fd, void *vptr, size_t max_len);


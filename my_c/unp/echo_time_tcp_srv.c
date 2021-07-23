/**
 * 返回时间的服务器程序
 */

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/errno.h>

#define MAX_LINE 1024
#define LISTENQ 1024

int writen(int fd, const void *vptr, int n) {
    int nleft;
    int nwritten;
    const char *ptr;

    ptr = (char *) vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (errno == EINTR) {
                nwritten = 0;
            } else {
                return (-1);
            }
        }

        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

int main(int argc, char **argv) {
    int listen_fd, conn_fd;
    struct sockaddr_in serv_addr;
    char buff[MAX_LINE];
    time_t ticks;

    // 修正clion printf不打印的问题
    setbuf(stdout, 0);

    // 创建一个套接字
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket error");
        exit(-1);
    }

    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9876);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listen_fd, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        printf("bind error");
        exit(-1);
    }

    if (listen(listen_fd, LISTENQ) < 0) {
        printf("listen error");
        exit(-1);
    }

    printf("time server running...\n");
    for (;;) {
        if ((conn_fd = accept(listen_fd, NULL, NULL)) < 0) {
            printf("accept error");
            exit(-1);
        }

        printf("New client connect id=%d\n", conn_fd);
        ticks = time(NULL);

        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        writen(conn_fd, buff, sizeof(buff));

        if (close(conn_fd) < 0) {
            printf("close error");
            exit(-1);
        }
    }
}
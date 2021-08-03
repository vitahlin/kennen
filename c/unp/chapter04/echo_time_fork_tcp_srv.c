/**
 * 返回时间的服务器程序
 * 采用fork函数并发处理
 */

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/errno.h>
#include <arpa/inet.h>
#include "../lib/constant.h"

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
    struct sockaddr_in serv_addr, cli_adddr;
    socklen_t len;
    char buff[MAX_LINE];
    time_t ticks;
    pid_t pid;

    // 修正clion printf不打印的问题
    setbuf(stdout, 0);

    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(-1);
    }

    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9901);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listen_fd, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind error");
        exit(-1);
    }

    // 将套接字转换成一个监听套接字，这样来自客户端的外来连接就可以在该套接字上由内核接受
    if (listen(listen_fd, LISTENQ) < 0) {
        perror("listen error");
        exit(-1);
    }

    printf("time server running...\n");
    for (;;) {
        if ((conn_fd = accept(listen_fd, (struct sockaddr *) &cli_adddr, &len)) < 0) {
            perror("accept error");
            exit(-1);
        }

        // 采用fork函数并发处理
        if ((pid = fork()) < 0) {
            perror("fork error");
            exit(-1);
        } else if (pid == 0) {
            // 子进程处理客户端连接逻辑
            if (close(listen_fd) < 0) {
                perror("close error");
                exit(-1);
            }

            printf("New client connect IP=%s, port=%d, conn_id=%d\n",
                   inet_ntop(AF_INET, &cli_adddr.sin_addr, buff, sizeof(buff)),
                   ntohs(cli_adddr.sin_port),
                   conn_fd);
            ticks = time(NULL);

            snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
            writen(conn_fd, buff, sizeof(buff));
            if (close(conn_fd) < 0) {
                perror("close error");
                exit(-1);
            }
            exit(0);
        }

        if (close(conn_fd) < 0) {
            perror("close error");
            exit(-1);
        }
    }
}
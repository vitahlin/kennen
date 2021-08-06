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

#define MAX_SIZE 1024
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

void strEcho(int sock_fd) {
    ssize_t n;
    char buf[10];

    again:
    bzero(buf, 10);
    while ((n = read(sock_fd, buf, 10)) > 0) {
        printf("receive from cli char count==%zd, content=%s\n", n, buf);
        writen(sock_fd, buf, n);
    }
    if (n < 0 && errno == EINTR) {
        goto again;
    } else if (n < 0) {
        perror("read error");
    }
}


int main(int argc, char **argv) {
    int listen_fd, conn_fd;
    struct sockaddr_in serv_addr, cli_adddr;
    socklen_t len;
    char buff[MAX_SIZE];
    pid_t child_pid;

    // 修正clion printf不打印的问题
    setbuf(stdout, 0);

    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(-1);
    }

    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9876);
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

    printf("server running...\n");
    for (;;) {
        len = sizeof(cli_adddr);
        if ((conn_fd = accept(listen_fd, (struct sockaddr *) &cli_adddr, &len)) < 0) {
            perror("accept error");
            exit(-1);
        }

        // 采用fork函数并发处理
        if ((child_pid = fork()) < 0) {
            perror("fork error");
            exit(-1);
        } else if (child_pid == 0) {
            // 子进程处理客户端连接逻辑
            if (close(listen_fd) < 0) {
                perror("close error");
                exit(-1);
            }

            strEcho(conn_fd);
            exit(0);
        }

        if (close(conn_fd) < 0) {
            perror("close error");
            exit(-1);
        }
    }
}
/**
 * 简单从时间服务器获取时间的tcp客户端程序
 */

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define MAX_LINE 1024

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

static int my_read(int fd, char *ptr) {
    static int read_cnt = 0;
    static char *read_ptr;
    static char read_buf[MAX_LINE];

    if (read_cnt <= 0) {
        again:
        if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
            if (errno == EINTR) {
                goto again;
            }
            return (-1);
        } else if (read_cnt == 0) {
            return (0);
        }
        read_ptr = read_buf;
    }

    read_cnt--;
    *ptr = *read_ptr++;
    return (1);
}

int readline(int fd, void *vptr, int maxlen) {
    int n, rc;
    char c, *ptr;

    ptr = (char *) vptr;
    for (n = 1; n < maxlen; n++) {
        if ((rc = my_read(fd, &c)) == 1) {
            *ptr++ = c;
            if (c == '\n') {
                break;
            }
        } else if (rc == 0) {
            if (n == 1) {
                return (0);
            } else {
                break;
            }
        } else {
            return (-1);
        }
    }

    *ptr = 0;
    return (n);
}

int Readline(int fd, void *ptr, int maxlen) {
    int n;

    if ((n = readline(fd, ptr, maxlen)) < 0) {
        perror("Readline error");
    }
    return (n);
}

void strCli(FILE *fp, int sock_fd) {
    printf("begin input...");
    char sendline[MAX_LINE], recvline[MAX_LINE];

    while (fgets(sendline, MAX_LINE, fp) != NULL) {
        writen(sock_fd, sendline, strlen(sendline));
        if (Readline(sock_fd, recvline, MAX_LINE) == 0) {
            printf("strCli: server terminated permaturely");
            exit(-1);
        }
        fputs(recvline, stdout);
    }
}

int main(int argc, char **argv) {
    int sock_fd, n;
    struct sockaddr_in serv_addr;
    char receive_line[MAX_LINE + 1];

    char serv_ip[16];
    int port;

    printf("Input IP address: ");
    scanf("%s", &serv_ip);
    printf("Input port: ");
    scanf("%d", &port);

    // 创建一个套接字
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(-1);
    }

    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, serv_ip, &serv_addr.sin_addr) < 0) {
        perror("inet_pton error");
        exit(-1);
    }

    if (connect(sock_fd, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect error");
        exit(-1);
    }

    strCli(stdin, sock_fd);

    return 0;
}
/**
 * 返回时间的服务器程序
 * 采用fork函数并发处理，处理子进程的SIGCHLD信号
 */

#include "../lib/unp.h"

void waitChildProcess(int sig_no) {
    pid_t pid;
    int stat;

    pid = wait(&stat);
    printf(" child %d terminated\n", &pid);
    return;
}

void strEcho(int sock_fd) {
    ssize_t n;
    char buf[10];

    again:
    bzero(buf, 10);
    while ((n = read(sock_fd, buf, 10)) > 0) {
        printf("receive from cli char count==%zd, content=%s\n", n, buf);
        wrapWriten(sock_fd, buf, n);
    }
    if (n < 0 && errno == EINTR) {
        goto again;
    } else if (n < 0) {
        perror("read error");
    }
}


int main(int argc, char **argv) {
    int listen_fd, conn_fd;
    struct sockaddr_in serv_address, cli_address;
    socklen_t len;
    char buff[MAX_SIZE];
    pid_t child_pid;

    listen_fd = wrapSocket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_address, sizeof(serv_address));

    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(9876);
    serv_address.sin_addr.s_addr = htonl(INADDR_ANY);

    wrapBind(listen_fd, (const struct sockaddr *) &serv_address, sizeof(serv_address));

    // 将套接字转换成一个监听套接字，这样来自客户端的外来连接就可以在该套接字上由内核接受
    wrapListen(listen_fd, LISTENQ);

    // 处理信号 SIGCHLD
    wrapSignal(SIGCHLD, waitChildProcess);

    printf("server running...\n");
    for (;;) {
        len = sizeof(cli_address);
        conn_fd = wrapAccept(listen_fd, (struct sockaddr *) &cli_address, &len);

        // 采用fork函数并发处理
        if ((child_pid = fork()) < 0) {
            perror("fork error");
            exit(-1);
        } else if (child_pid == 0) {
            // 子进程处理客户端连接逻辑
            wrapClose(listen_fd);

            strEcho(conn_fd);
            exit(0);
        }
        wrapClose(conn_fd);
    }
}
/**
 * 计算两个数和的服务端程序
 * 客户端输入的两个数用空格隔开
 */
#include "../lib/unp.h"

void waitpidChildProcess(int sig_no) {
    pid_t pid;
    int stat;

    // 指定 WNOHANG 选项，告知当有尚未终止当子进程在运行时不要阻塞
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        printf("child %ld terminated\n", (long) pid);
    }
    return;
}

void calculateSum(int sock_fd) {
    ssize_t n;
    char receive_line[MAX_SIZE];
    long arg1, arg2;

    for (;;) {
        if ((n = wrapReadlineV2(sock_fd, receive_line, MAX_SIZE)) == 0) {
            return;
        }

        if (sscanf(receive_line, "%ld%ld", &arg1, &arg2) == 2) {
            snprintf(receive_line, sizeof(receive_line), "%ld\n", arg1 + arg2);
        } else {
            snprintf(receive_line, sizeof(receive_line), "Input error\n");
        }
        n = strlen(receive_line);
        wrapWriten(sock_fd, receive_line, n);
    }
}


int main(int argc, char **argv) {
    int listen_fd, conn_fd;
    struct sockaddr_in serv_address, cli_address;
    socklen_t len;
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
    wrapSignal(SIGCHLD, waitpidChildProcess);

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

            calculateSum(conn_fd);
            exit(0);
        }
        wrapClose(conn_fd);
    }
}
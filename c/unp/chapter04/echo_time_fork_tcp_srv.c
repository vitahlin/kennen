/**
 * 返回时间的服务器程序
 * 采用fork函数并发处理
 */

#include <time.h>
#include "../lib/unp.h"

int main(int argc, char **argv) {
    int listen_fd, conn_fd;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t len;
    char buff[MAX_SIZE];
    time_t ticks;
    pid_t pid;

    listen_fd = wrapSocket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9901);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    wrapBind(listen_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    // 将套接字转换成一个监听套接字，这样来自客户端的外来连接就可以在该套接字上由内核接受
    wrapListen(listen_fd, LISTENQ);

    printf("time server running...\n");
    for (;;) {
        conn_fd = wrapAccept(listen_fd, (struct sockaddr *) &cli_addr, &len);

        // 采用fork函数并发处理
        if ((pid = fork()) < 0) {
            perror("fork error");
            exit(-1);
        } else if (pid == 0) {
            // 子进程处理客户端连接逻辑
            wrapClose(listen_fd);

            printf("New client connect IP=%s, port=%d, conn_id=%d\n",
                   inet_ntop(AF_INET, &cli_addr.sin_addr, buff, sizeof(buff)),
                   ntohs(cli_addr.sin_port),
                   conn_fd);
            ticks = time(NULL);

            snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
            wrapWriten(conn_fd, buff, sizeof(buff));
            wrapClose(conn_fd);
            exit(0);
        }
        wrapClose(conn_fd);
    }
}
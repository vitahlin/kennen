/**
 * 返回时间的服务器程序
 */
#include <time.h>
#include "../lib/constant.h"
#include "../lib/unp.h"

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
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t len;
    char buff[MAX_SIZE];
    time_t ticks;

    // 修正clion printf不打印的问题
    setbuf(stdout, 0);

    listen_fd = wrapSocket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9876);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    wrapBind(listen_fd, (const struct sockaddr *) &serv_addr, sizeof(serv_addr));

    // 将套接字转换成一个监听套接字，这样来自客户端的外来连接就可以在该套接字上由内核接受
    wrapListen(listen_fd, LISTENQ);

    printf("time server running...\n");
    for (;;) {
        conn_fd = wrapAccept(listen_fd, (struct sockaddr *) &cli_addr, len);

        printf("New client connect IP=%s, port=%d, conn_id=%d\n",
               inet_ntop(AF_INET, &cli_addr.sin_addr, buff, sizeof(buff)),
               ntohs(cli_addr.sin_port),
               conn_fd);
        ticks = time(NULL);

        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        wrapWriten(conn_fd, buff, sizeof(buff));

        wrapClose(conn_fd);
    }
}
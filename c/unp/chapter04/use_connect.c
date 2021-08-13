/**
 * connect函数使用示例
 * 错误有3种情况
 * 1. 服务器根本无法连接，如输入一个不存在的IP
 * 2. 服务器端口没有对应的进程响应，输入真实的IP、错误的端口
 * 3. 服务不可达，指定一个因特网中不可到底的IP地址
 */
#include <time.h>
#include "../lib/unp.h"

int main(int argc, char **argv) {
    int sock_fd;
    struct sockaddr_in serv_addr;
    clock_t start, finish;

    char serv_ip[16];
    int port;

    setbuf(stdout, 0);

    printf("Input IP address: ");
    scanf("%s", (char *) &serv_ip);
    printf("Input port: ");
    scanf("%d", &port);

    // 创建一个套接字
    sock_fd = wrapSocket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, serv_ip, &serv_addr.sin_addr) < 0) {
        printf("inet_pton error for %s", serv_ip);
        exit(-1);
    }

    start = clock();
    if (connect(sock_fd, (const struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        finish = clock();
        double duration = (double) (finish - start) / CLOCKS_PER_SEC;
        printf("connect time: %fs\n", duration);
        perror("connect error");
        exit(-1);
    }

    finish = clock();
    double duration = (double) (finish - start) / CLOCKS_PER_SEC;
    printf("connect time: %f\nconnect success\n", duration);

    return 0;
}


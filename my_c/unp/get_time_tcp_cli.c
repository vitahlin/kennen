/**
 * 简单从时间服务器获取时间的tcp客户端程序
 */

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_LINE 1024

int main(int argc, char **argv) {
    int sock_fd, n;
    struct sockaddr_in serv_address;
    char receive_line[MAX_LINE + 1];

    // 可用的时间服务器IP地址
    char time_serv_ip[12] = "132.163.96.5";

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("socket error");
        exit(-1);
    }

    bzero(&serv_address, sizeof(serv_address));

    serv_address.sin_family = AF_INET;

    // 指定端口，时间服务器端口13
    serv_address.sin_port = htons(13);

    if (inet_pton(AF_INET, time_serv_ip, &serv_address.sin_addr) < 0) {
        printf("inet_pton error for %s", argv[1]);
        exit(-1);
    }

    if (connect(sock_fd, (const struct sockaddr *) &serv_address, sizeof(serv_address)) < 0) {
        printf("connect error");
        exit(-1);
    }

    while ((n = read(sock_fd, receive_line, MAX_LINE)) > 0) {
        receive_line[n] = 0;
        if (fputs(receive_line, stdout) == EOF) {
            printf("fputs error");
            exit(-1);
        }
    }

    if (n < 0) {
        printf("read error");
        exit(-1);
    }

    return 0;
}
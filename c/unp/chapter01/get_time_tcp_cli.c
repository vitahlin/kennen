/**
 * 简单从时间服务器获取时间的tcp客户端程序
 */

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "../lib/constant.h"

int main(int argc, char **argv) {
    int sock_fd, n;
    struct sockaddr_in serv_address;
    char receive_line[MAX_SIZE + 1];

    char serv_ip[16];
    int port;

    // 网络上可用的时间服务器IP地址132.163.96.5，端口13
    printf("Input IP address: ");
    scanf("%s", &serv_ip);
    printf("Input port: ");
    scanf("%d", &port);

    // 创建一个套接字
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket error");
        exit(-1);
    }

    bzero(&serv_address, sizeof(serv_address));

    serv_address.sin_family = AF_INET;

    // 指定端口，时间服务器端口13
    serv_address.sin_port = htons(port);

    if (inet_pton(AF_INET, serv_ip, &serv_address.sin_addr) < 0) {
        printf("inet_pton error for %s", serv_ip);
        exit(-1);
    }

    if (connect(sock_fd, (const struct sockaddr *) &serv_address, sizeof(serv_address)) < 0) {
        perror("connect error");
        exit(-1);
    }

    while ((n = read(sock_fd, receive_line, MAX_SIZE)) > 0) {
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
/**
 * 函数 inet_pton 和 inet_ntop 的示例
 * inet_pton 将IP从点分十进制转为二进制
 * inet_ntop 将IP地址从二进制转为点分十进制
 */
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
    char ip[20];
    struct in_addr s;

    printf("input IP address:");
    scanf("%s", ip);

    // 转换
    inet_pton(AF_INET, ip, &s);
    printf("inet_pton: 0x%x\n", s.s_addr);

    // 反转换
    inet_ntop(AF_INET, &s, ip, 16);
    printf("inet_ntop: %s\n", ip);

    return 0;
}
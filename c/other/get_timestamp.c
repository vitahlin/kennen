/**
 * 获取时间戳
 */
#include <stdio.h>
#include <sys/time.h>

int main() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    printf("秒: %ld\n", tv.tv_sec);
    printf("毫秒: %ld\n", tv.tv_sec * 1000 + tv.tv_usec / 1000);
    printf("微秒: %ld\n", tv.tv_sec * 1000000 + tv.tv_usec);

    return 0;
}
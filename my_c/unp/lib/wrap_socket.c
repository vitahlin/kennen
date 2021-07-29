
#include "./unp.h"

/**
 * 封装的socket函数
 * @param domain
 * @param type
 * @param protocol
 * @return
 */
int wrapSocket(int domain, int type, int protocol) {
    int n;
    if ((n = socket(domain, type, protocol)) < 0) {
        perror("socket error");
        exit(-1);
    }

    return n;
}
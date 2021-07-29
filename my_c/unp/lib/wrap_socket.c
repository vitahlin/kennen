
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
        perror("socket error ");
        exit(-1);
    }

    return n;
}


/**
 * 封装的close函数
 * @param sock_fd
 */
void wrapClose(int sock_fd) {
    if (close(sock_fd) == 0) {
        return;
    }
    perror("close error ");
    exit(-1);
}
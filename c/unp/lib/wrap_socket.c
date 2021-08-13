
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

void wrapConnect(int sock_fd, const struct sockaddr *serv_addr, socklen_t addr_length) {
    if (connect(sock_fd, serv_addr, addr_length) < 0) {
        perror("connect error ");
        exit(-1);
    }
}

int wrapAccept(int sock_fd, struct sockaddr *cli_address, socklen_t *cli_address_length) {
    int n;
    if ((n = accept(sock_fd, cli_address, cli_address_length)) < 0) {
        perror("accept error ");
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

/**
 * 封装bind函数
 * @param sock_fd
 * @param address
 * @param sock_len
 */
void wrapBind(int sock_fd, const struct sockaddr *address, socklen_t sock_len) {
    if (bind(sock_fd, address, sock_len) < 0) {
        perror("bind error ");
        exit(-1);
    }
}

void wrapListen(int sock_fd, int listen_num) {
    if (listen(sock_fd, listen_num) < 0) {
        perror("listen error ");
        exit(-1);
    }
}

void wrapInetPton(int domain, const char *ip, void *dst) {
    if (inet_pton(AF_INET, ip, dst) < 0) {
        printf("inet_pton error for %s", ip);
        exit(-1);
    }
}

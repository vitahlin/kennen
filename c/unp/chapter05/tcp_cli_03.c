#include "../lib/unp.h"

void strCli(FILE *fp, int sock_fd) {
    char send_line[MAX_SIZE], receive_line[MAX_SIZE];
    while (wrapFgets(send_line, MAX_SIZE, fp) != NULL) {

        // 这里是为了让第一次wrapWriten引发一个RST，再让第二个wrapWriten产生SIGPIPE信号
        wrapWriten(sock_fd, send_line, 1);
        sleep(1);
        wrapWriten(sock_fd, send_line + 1, strlen(send_line) - 1);

        if (wrapReadlineV2(sock_fd, receive_line, MAX_SIZE) == 0) {
            printf("strCli: server terminated prematurely");
            exit(-1);
        }
        printf("Receive from server:");
        wrapFputs(receive_line, stdout);
    }
}

int main(int argc, char **argv) {
    int sock_fd;
    struct sockaddr_in serv_address;

    char serv_ip[16] = "127.0.0.1";
    int port = 9876;

    // 创建一个套接字
    sock_fd = wrapSocket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_address, sizeof(serv_address));

    serv_address.sin_family = AF_INET;
    serv_address.sin_port = htons(port);

    wrapInetPton(AF_INET, serv_ip, &serv_address.sin_addr);
    wrapConnect(sock_fd, (const struct sockaddr *) &serv_address, sizeof(serv_address));

    strCli(stdin, sock_fd);

    return 0;
}
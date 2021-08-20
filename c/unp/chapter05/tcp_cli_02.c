#include "../lib/unp.h"

void strCli(FILE *fp, int sock_fd) {
    char send_line[MAX_SIZE], receive_line[MAX_SIZE];
    while (wrapFgets(send_line, MAX_SIZE, fp) != NULL) {
        wrapWriten(sock_fd, send_line, strlen(send_line));
        if (wrapReadlineV2(sock_fd, receive_line, MAX_SIZE) == 0) {
            printf("strCli: server terminated prematurely");
            exit(-1);
        }
        printf("Receive from server:");
        wrapFputs(receive_line, stdout);
    }
}

int main(int argc, char **argv) {
    int i;
    int sock_fd[5];
    struct sockaddr_in serv_address;

    char serv_ip[16] = "127.0.0.1";
    int port = 9876;

    // 建立5个连接
    for (i = 0; i < 5; i++) {
        sock_fd[i] = wrapSocket(AF_INET, SOCK_STREAM, 0);
        bzero(&serv_address, sizeof(serv_address));
        serv_address.sin_family = AF_INET;
        serv_address.sin_port = htons(port);

        wrapInetPton(AF_INET, serv_ip, &serv_address.sin_addr);
        wrapConnect(sock_fd[i], (const struct sockaddr *) &serv_address, sizeof(serv_address));
    }

    strCli(stdin, sock_fd[0]);

    return 0;
}
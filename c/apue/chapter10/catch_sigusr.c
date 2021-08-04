/**
 * 捕捉SIGUSR1和SIGUSR2信号的示例
 * 使程序在后台运行，终端中输入：./catch_sigusr & ，会打印当前进程ID xxx
 * 然后用kill命令将信号发送给进程，输入：
 * kill -USR1 xxx 发送USR1信号
 * kill -USR2 xxx 发送USR2信号
 */
#include "../lib/apue.h"

static void SigUsrHandler(int);

int main(int argc, const char** argv) {
    if (signal(SIGUSR1, SigUsrHandler) == SIG_ERR) {
        perror("can not catch SIGUSR1");
        exit(1);
    }

    if (signal(SIGUSR2, SigUsrHandler) == SIG_ERR) {
        perror("can not catch SIGUSR2");
        exit(1);
    }

    for (;;) {
        /**
         * 使进程（或者线程）睡眠状态，直到接收到信号，要么终止，或导致它调用一个信号捕获函数
         */
        pause();
    }

    return 0;
}

/**
 * 信号处理函数
 * @param sig_num
 */
static void SigUsrHandler(int sig_num) {
    if (sig_num == SIGUSR1) {
        printf("catch signal SIGUSR1\n");
    } else if (sig_num == SIGUSR2) {
        printf("catch signal SIGUSR2\n");
    } else {
        printf("catch signal %d\n", sig_num);
    }
}
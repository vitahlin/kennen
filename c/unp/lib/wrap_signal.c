
#include "./unp.h"

/**
 * 封装的signal函数
 * @param sig_no 信号
 * @param func 处理函数
 * @return
 */
SignalHandler *wrapSignal(int sig_no, SignalHandler *func) {
    SignalHandler *sig_func;
    if ((sig_func = signal(sig_no, func)) == SIG_ERR) {
        perror("signal error ");
        exit(-1);
    }

    return (sig_func);
}
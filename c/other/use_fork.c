/**
 * fork()函数的使用
 * 子进程对变量对修改并不影响父进程中该变量的值
 */

#include <stdio.h>
#include <unistd.h>

int global_var = 66;

int main() {
    int var = 88;
    pid_t pid;

    if ((pid = fork()) < 0) {
        perror("fork error");
        _exit(-1);
    } else if (pid == 0) {
        // 子进程
        var++;
        global_var++;
    } else {
        /**
         * 父进程执行分支
         * fork之后，父进程还是子进程先执行无法确定，这里父进程休眠2秒以让子进程先执行
         */
        sleep(2);
    }

    printf("pid=%d, global var=%d, var=%d\n", getpid(), global_var, var);

    return 0;
}
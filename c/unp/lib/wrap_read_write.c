
#include "./unp.h"
#include "./constant.h"

/**
 * 套接字的read和write函数不同于通常的文件I/O。套接字上调用read或者write输入或者输出的字节数可能比请求的数量少，
 * 这不是出错的状态，原因在于内核中用于套接字的缓冲区已经到达了极限，此时需要再次调用read或者write函数，以输入输出
 * 剩余的字节。
 */

/**
 * 从一个描述符读取n字节
 * @param fd
 * @param buff
 * @param n
 * @return 读取的字节数，出错则为-1
 */
ssize_t wrapReadn(int fd, void *buff, size_t n) {
    size_t nleft;
    ssize_t read_num;
    char *ptr;

    ptr = buff;
    nleft = n;
    while (nleft > 0) {
        if ((read_num = read(fd, ptr, nleft)) < 0) {
            if (errno == EINTR) {
                // 产生中断则继续调用read()函数
                read_num = 0;
            } else {
                return -1;
            }
        } else if (read_num == 0) {
            // EOF
            break;
        }

        nleft -= read_num;
        ptr += read_num;
    }

    return (n - nleft);
}

/**
 * 往一个描述符写n字节
 * @param fd
 * @param vptr
 * @param n
 * @return 写入的字节数
 */
ssize_t wrapWriten(int fd, const void *vptr, size_t n) {
    size_t nleft;
    ssize_t written_n;
    const char *ptr;
    ptr = vptr;
    nleft = n;

    while (nleft > 0) {
        if ((written_n = write(fd, ptr, nleft)) <= 0) {
            if (written_n < 0 && errno == EINTR) {
                written_n = 0;
            } else {
                return -1;
            }
        }

        nleft -= written_n;
        ptr += written_n;
    }

    return n;
}

/**
 * 从一个描述符读文本行，一次一个字节
 *
 * 每读取一个字节的数据就调用一个系统的read()函数，这是非常低效率的。
 * 当从某个套接字读入文本行时，改用标准库I/O却又是危险的，因为stdio缓冲区的状态是不可见的。
 * @param fd
 * @param vptr
 * @param max_len
 * @return
 */
ssize_t wrapReadlineV1(int fd, void *vptr, size_t max_len) {
    ssize_t n, rc;
    char c, *ptr;
    ptr = vptr;

    for (n = 1; n < max_len; n++) {
        again:
        if ((rc = read(fd, &c, 1)) == 1) {
            *ptr++ = c;
            if (c == '\n') {
                break;
            }
        } else if (rc == 0) {
            *ptr = 0;
            return n - 1;
        } else {
            if (errno == EINTR) {
                goto again;
            }
            return -1;
        }
    }

    *ptr = 0;
    return n;
}

static int read_cnt;
static char *read_ptr;
static char read_buf[MAX_SIZE];

/**
 * 每次最多读取MAX_SIZE个自负，然后每次返回一个字符
 * @param fd
 * @param ptr
 * @return
 */
static ssize_t readV2(int fd, char *ptr) {
    if (read_cnt <= 0) {
        again:
        if ((read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
            if (errno == EINTR) {
                goto again;
            }
            return -1;
        } else if (read_cnt == 0) {
            return 0;
        }
        read_ptr = read_buf;
    }

    read_cnt--;
    *ptr = *read_ptr++;
    return 1;
}

/**
 * 优化了wrapReadlineV1，使用自己的缓冲机制
 * @param fd
 * @param vptr
 * @param max_len
 * @return
 */
ssize_t wrapReadlineV2(int fd, void *vptr, size_t max_len) {
    ssize_t n, rc;
    char c, *ptr;
    ptr = vptr;

    for (n = 1; n < max_len; n++) {
        if ((rc = readV2(fd, &c)) == 1) {
            *ptr++ = c;
            if (c == '\n') {
                break;
            }
        } else if (rc == 0) {
            *ptr = 0;
            return n - 1;
        } else {
            return -1;
        }
    }

    *ptr = 0;
    return n;
}


#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/file.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <getopt.h>
#include "uart.h"

static int My_read_result(int fd, char *buf, int max_read, int time)
{
    int len = 0;
    char *recv_msg = buf;
    char *ptr = recv_msg;
    int out = 0;
    if (max_read <= 0)
        max_read = 4096;
    while (out < time) {
        int ret = 0;
        if ((ret = MyuartRxExpires(fd, max_read, (uint8_t *)ptr, 500)) > 0) {
            len += ret;
            ret = 0;
            if ((len >= max_read) || ((ptr + len - buf) >= max_read)) {
                recv_msg[max_read - 1] = '\0';
                return len;
            }
            if (strstr(recv_msg, "OK") || strstr(recv_msg, "ERROR")) {
                recv_msg[len] = '\0';
                recv_msg[max_read - 1] = '\0';
                return len;
            } else {
                ptr = ptr + len;
                continue;
            }
        }
        out++;
    }
    recv_msg[max_read - 1] = '\0';
    return len;
}



int send_command(const char *device, const char *cmd, char *recv_buf, int max_read)
{
    int ret = 0;
    int time_out = 0;
    int fd;
    char cmd_withln[1026];
    int time = 3;

    strcpy(cmd_withln, cmd);
    strcat(cmd_withln, "\r\n");

    fd = uartOpen((int8_t *)device, 115200, 0, 100);
    if (fd < 0) {
        return -1;
    }

    while (flock(fd, LOCK_EX | LOCK_NB) != 0) { //get file lock
        if (++time_out > 3000) {//time out 3s
            MyuartClose(fd);
            return -2;
        }
        usleep(1000);
    }
    MyflushIoBuffer(fd);
    MyuartTxNonBlocking(fd, strlen(cmd_withln), cmd_withln);
    if (0 == strncmp(cmd_withln, "AT+COPS=?", 9)) {
        time = 360;
    }

    ret = My_read_result(fd, recv_buf, max_read, time);
    MyuartClose(fd);

    if (ret > 0)
        return 0;
    else
        return -1;
}






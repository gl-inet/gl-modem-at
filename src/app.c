#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
#include <getopt.h>

extern int send_command(const char *device, const char *cmd, char *recv_buf, int max_read);

int main(int argc, char *argv[])
{
    int  i = 0, j = 0;
    /*In keeping with the format of glinet's built-in AT instruction program, we omit some parameters*/
    if (argc > 1)
        if (0 == strcmp(argv[1], "-D")) {
            for (i = 1; i < argc; i++) {
                argv[i] = argv[i + 1];
            }
            argc--;
        }

    if (argc > 2)
        if (0 == strcmp(argv[1], "-B")) {
            for (j = 0; j < 2; j++) {
                for (i = 1; i < argc; i++) {
                    argv[i] = argv[i + 1];
                }
                argc--;
            }
        }

    if (argc > 1)
        if (0 == strcmp(argv[1], "AT")) {
            for (i = 1; i < argc; i++) {
                argv[i] = argv[i + 1];
            }
            argc--;
        }

    if (argc < 3) {
        printf("\nUsage:<gl_modem> [device] <command>\n");
        return 1;
    }
    char recv[4096] = { 0 };
    char cmd[1024] = {0};
    char dev[128] = {0};
    strncpy(dev, argv[1], sizeof(dev) - 1);
    strncpy(cmd, argv[2], sizeof(cmd) - 1);

    if (0 == send_command(dev, cmd, recv, sizeof(recv))) {
        printf("\n%s", recv);
    }
    exit(0);
}

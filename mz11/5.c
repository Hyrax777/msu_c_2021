#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>


enum
{
    START_VL = 7,
};

volatile int count_bit = 7;
volatile sig_atomic_t sending = 1;
char output;
int pid;

void 
handler_get_bit1(int s) {
    output = output | (1 << count_bit--);
    sending = 1;
}

void
handler_get_bit0(int s) {
    count_bit--;
    sending = 1;
}

void
handler_finish(int s) {
    exit(0);
}

void
handler_alarm(int s) {
    sending = 0;
}

int main(int argc, char **argv) {
    char temp;
    sigaction(SIGALRM, &(struct sigaction){ .sa_handler = handler_alarm, .sa_flags = SA_RESTART }, NULL);
    pid = fork();
    if (pid < 0) {
        exit(1);
    } else if (!pid) {
        sigaction(SIGIO, &(struct sigaction){ .sa_handler = handler_finish, .sa_flags = SA_RESTART }, NULL);
        sigaction(SIGUSR1, &(struct sigaction){ .sa_handler = handler_get_bit0, .sa_flags = SA_RESTART }, NULL);
        sigaction(SIGUSR2, &(struct sigaction){ .sa_handler = handler_get_bit1, .sa_flags = SA_RESTART }, NULL);
        signal(SIGALRM, SIG_IGN);  //игнор sigalarm
        kill(0, SIGALRM);
        sending = 0;
        while (1) {
            while (!sending) {
                pause();
            }
            sending = 0;
            if (count_bit == -1) {
                printf("%c", output); fflush(stdout);
                count_bit = 7;
                output = 0;
            }
            kill(0, SIGALRM);
        }
        exit(0);
    }

    int pid2 = fork();
    if (pid2 < 0) {
        exit(1);
    } else if (!pid2) {
        while (sending) {
            pause();
        }
        int fd = open(argv[1], O_RDONLY);
        while (read(fd, &temp, sizeof(temp) == sizeof(temp))) {
            for (int i = 0; i < 8; i++) {
                int cur = temp & (1 << (7 - i));
                sending = 1;
                cur ? kill(pid, SIGUSR2) : kill(pid,  SIGUSR1);
                while (sending) {
                    pause();
                }
            }
        }
        close(fd);
        kill(pid, SIGIO);
        exit(0);
    }

    signal(SIGALRM, SIG_IGN);  //игнор sigalarm
    while (wait(NULL) > 0) {}
    return 0;
}


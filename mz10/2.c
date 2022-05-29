#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

enum
{
    ST_ADD = 0,
    ST_MUL = 1,
};


volatile int st = ST_ADD;

void handler1(int s) {
    st = ST_ADD;
}

void handler2(int s) {
    st = ST_MUL;
}


int main() {
    unsigned int sum = 0;
    int temp;
    sigaction(SIGINT, &(struct sigaction) { .sa_handler = handler1, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGQUIT, &(struct sigaction) { .sa_handler = handler2, .sa_flags = SA_RESTART }, NULL);
    pid_t pid = getpid();
    printf("%d\n", pid); fflush(stdout);
    while (scanf("%d", &temp) > 0) {
        if (st) {
            sum *=temp;
        } else {
            sum += temp;
        }
        printf("%d\n", sum); fflush(stdout);
    }
    return 0;
}
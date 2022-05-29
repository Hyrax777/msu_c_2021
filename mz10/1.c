#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

static int count = 0;

enum
{
    STOP = 5, //конец
};

void 
handler(int s) 
{
    printf("%d\n", count++); fflush(stdout);
    if (count == STOP) {
        exit(0);
    }
}

int main() {
    sigaction(SIGHUP, &(struct sigaction) { .sa_handler = handler, .sa_flags = SA_RESTART }, NULL);
    pid_t pid = getpid();
    printf("%d\n", pid); fflush(stdout);
    while (1) {
        pause();
    }
}

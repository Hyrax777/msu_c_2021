#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

volatile sig_atomic_t usr_flag;

enum
{
    BASE = 10, //система счисления
};

void 
handler(int s)
{
    usr_flag = 1;
}

void 
work(int serial, FILE* rfd, FILE* wfd, int stop)
{
    int pid;
    sigset_t s2;
    sigemptyset(&s2);
    while (1) {
        while (!usr_flag) {
            sigsuspend(&s2);
        }
        usr_flag = 0;
        int val;
        if (fscanf(rfd, " %d%d", &pid,  &val) <= 0) {
            break;
        }
        if (val < stop) {
            printf("%d %d\n", serial, val++); 
            fflush(stdout);
        }
        fprintf(wfd, "%d %d\n", getpid(), val); 
        fflush(wfd);
        kill(pid, SIGUSR1);
        if (val >= stop) {
            break;
        }
    }
}

int main(int argc, char** argv) {
    int fds[2],  start = 1, pid1, pid2;
    pipe(fds);
    FILE *rf = fdopen(fds[0], "r");
    FILE *wf = fdopen(fds[1], "w");
    int stop = strtol(argv[1], NULL, BASE);
    sigset_t s1;
    sigemptyset(&s1);
    sigaddset(&s1, SIGUSR1);
    sigprocmask(SIG_BLOCK, &s1, NULL);
    if (start < stop) {
        pid1 = fork();
        if (!pid1) {
            sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler }, NULL);
            work(1, rf, wf, stop);
            fclose(rf); fclose(wf);
            _exit(0);
        }
        pid2 = fork();
        if (!pid2) {
            sigaction(SIGUSR1, &(struct sigaction) { .sa_handler = handler }, NULL);
            work(2, rf, wf, stop);
            fclose(rf); fclose(wf);
            _exit(0);
        }
        fprintf(wf, "%d %d\n", pid2, start); fflush(wf);
        kill(pid1, SIGUSR1);
        fclose(rf); fclose(wf);
        while (wait(NULL) > 0) {}
    }
    printf("Done\n");  
    return 0;
}


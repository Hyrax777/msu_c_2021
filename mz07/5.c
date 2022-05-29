#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>


int main() {
    int temp;
    int start_pid = getpid();
    int pid = 0;
    if (pid == 0) {
        while (scanf("%d", &temp) != EOF) {
            int status;
            int pid = fork();
            if (pid > 0) {
                wait(&status);
                if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                    printf("%d\n", temp);
                }
                if (getpid() == start_pid) {
                    exit(0);
                }
                exit(WEXITSTATUS(status));
            } else if (pid == -1) {
                printf("-1\n");
                exit(1);
            }
        }
    }
    return 0;
}
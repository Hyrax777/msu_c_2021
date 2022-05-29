#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

int main() {
    int fds[2], temp;
    pipe(fds);
    int pid = fork();
    if (pid < 0) {
        exit(1);
    } else if (!pid) {
        pid = fork();
        if (pid < 0) {
            exit(1);
        } else if (!pid) {
            close(fds[1]);
            int cur;
            long long sum = 0;
            while (read(fds[0], &cur, sizeof(cur)) == sizeof(cur)) {
                sum += cur;
            }
            printf("%lld\n", sum);
            fflush(stdout);
            _exit(0);
        } else if (pid > 0) {
            close(fds[0]);
            close(fds[1]);
        }
    } else if (pid > 0) {
        close(fds[0]);
        while (scanf("%d", &temp) != EOF) {
            write(fds[1], &temp, sizeof(temp)); 
        }
        close(fds[1]);
        while (wait(NULL) > 0) {}
        exit(0);
    }
    wait(NULL);
    return 0;
}
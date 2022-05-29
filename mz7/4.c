#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main() {
    int i, n;
    scanf("%d", &n);
    for (i = 1; i < n; i++) {
        if (i != n) {
            printf("%d ", i);
        } else {
            printf("%d\n", i);
        }
        fflush(stdout);
        if (fork() > 0) {
            wait(NULL);
            exit(0);
        }
    }
    printf("%d\n", i);
    return 0;
}
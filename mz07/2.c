#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

enum
{
    NUMB = 3,
};

int main() {
    int i;
    for (i = 1; i < NUMB; i++) {
        if (fork() > 0) {
            wait(NULL);
            if (i != 1) {
                printf("%d ", i);
            } else {
                printf("%d\n", i);
            }
            fflush(stdout);
            exit(0);
        }
    }
    printf("%d ", i);
    return 0;
}
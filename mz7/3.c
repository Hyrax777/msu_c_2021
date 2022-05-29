#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

enum
{
    NUMB = 3,
    BASE = 10, // система счисления
    SIZE = 9
};

int main() {
    int i, cur;
    char temp[SIZE] = {0}, *end;
    setbuf(stdin, NULL);
    for (i = 1; i <= NUMB; i++) {
        if (fork() == 0) {
            if (read(STDIN_FILENO, temp, sizeof(temp) - 1) <= 0) {
                return 1;
            }
            cur = strtol(temp, &end, BASE);
            printf("%d %d\n", i, cur * cur);
            fflush(stdout);
            exit(0);
        }
    }
    while (wait(NULL) > 0) {}
    return 0;
}
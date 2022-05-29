#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>
#include <string.h>

int min(int a, int b) {
    return (a < b ? a : b);
}


void
func(const char* input_str, int pid) {
    if (!pid) {
        FILE* fd = fopen(input_str, "r");
        if (fd < 0) {
            exit(1);
        }
        char str[PATH_MAX + 1];
        if (fgets(str, PATH_MAX + 1, fd) == NULL) {
            exit(1);
        }
        int len = strlen(str);
        str[len - 1] = '\0';
        fclose(fd);
        execl(str, str, NULL);
        _exit(1);
    }
}


int main(int argc, char** argv) {
    int n = strtol(argv[1], NULL, 10), i, count = 0;
    int* arr_pid = calloc(argc - 2, sizeof(n));
    for (i = 0; i < min(n, argc - 2); i++) {
        arr_pid[i] = fork();
        if (arr_pid[i] > 0) {
            int status;
            waitpid(arr_pid[i], &status, 0);
            count += WIFEXITED(status) && !WEXITSTATUS(status);
        }
        func(argv[i + 2], arr_pid[i]);
    }
    for (i = n; i < argc - 2; i++) {
        arr_pid[i] = fork();
        if (arr_pid[i] < 0) {
            continue;
        }
        func(argv[i + 2], arr_pid[i]);
        int status;
        waitpid(arr_pid[i], &status, 0);
        count += WIFEXITED(status) && !WEXITSTATUS(status);
    }
    printf("%d\n", count); fflush(stdout);
    free(arr_pid);
    return 0;
}
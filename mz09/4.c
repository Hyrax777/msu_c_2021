#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

void 
kill_ch(int n, int* arr_pid) 
{
    for (int i = 0; i < n; i++) {
        kill(arr_pid[i], SIGKILL);
        waitpid(arr_pid[i], NULL, 0);
    }
}


int main(int argc, char** argv) {
    if (argc == 1) {
        return 0;
    } else if (argc == 2) {
        int pid = fork();
        if (pid < 0) {
            exit(1);
        } else if (!pid) {
            execlp(argv[1], argv[1], NULL);
            _exit(1);    
        }
        wait(NULL);
        return 0;
    }
    int p1[2], p2[2], i;
    int* arr_pid = calloc(argc - 1, sizeof(pid_t));
    if (pipe(p1) == -1) {
        free(arr_pid);
        exit(1);
    }
    arr_pid[0] = fork();
    if (arr_pid[0] < 0) {
        free(arr_pid);
        _exit(1);
    } else if (!arr_pid[0]) {
        dup2(p1[1], 1);
        close(p1[0]); close(p1[1]);
        execlp(argv[1], argv[1], NULL);
        free(arr_pid);
        _exit(1);
    }
    p2[0] = p1[0];
    p2[1] = p1[1];
    for (i = 2; i < argc - 1; i++) {
        if (pipe(p2) == -1) {
            kill_ch(i, arr_pid);
            free(arr_pid);
            exit(1);
        }
        arr_pid[i - 1] = fork();
        if (arr_pid[i - 1] < 0) {
            kill_ch(i, arr_pid);
            free(arr_pid);
            _exit(1);
        } else if (!arr_pid[i - 1]) {
            if (dup2(p1[0], 0) == -1) {
                _exit(1);
            }
            close(p1[0]); close(p1[1]);
            if (dup2(p2[1], 1) == -1) {
                _exit(1);
            }
            close(p2[0]); close(p2[1]);
            execlp(argv[i], argv[i], NULL);
            free(arr_pid);
            _exit(1);
        }
        close(p1[0]); close(p1[1]);
        p1[0] = p2[0];
        p1[1] = p2[1];
    }
    arr_pid[argc - 2] = fork();
    if (arr_pid[argc - 2] < 0) {
        kill_ch(argc - 2, arr_pid);
        free(arr_pid);
        _exit(1);
    } else if (!arr_pid[argc - 2]) {
        if (dup2(p1[0], 0) == -1) {
                _exit(1);
            }
        close(p2[0]); close(p2[1]);
        execlp(argv[argc - 1], argv[argc - 1], NULL);
        free(arr_pid);
        _exit(1);
    }
    close(p2[0]); close(p2[1]);
    while (wait(NULL) > 0) {}
    free(arr_pid);
    return 0;
}
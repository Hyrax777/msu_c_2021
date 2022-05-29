#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>


int main(int argc, char** argv) {
    int pfd[2]; 
    pipe(pfd);
    pid_t pid1 = fork();
    if (!pid1) {
        close(pfd[0]);
        if (dup2(pfd[1], 1) == -1) {
            _exit(1);
        }
        pid_t pid2 = fork();
        if (!pid2) {
            int fd = open(argv[4], O_RDONLY);
            if (fd < 0) {
                _exit(1);
            }
            if (dup2(fd, 0) == -1) { 
                _exit(1);
            } 
            close(fd);   
            execlp(argv[1], argv[1], NULL);
            _exit(1);
        }
        int status;
        waitpid(pid2, &status, 0);
        if (WIFEXITED(status) && !WEXITSTATUS(status)) {
            int pid3 = fork();
            if(!(pid3)) {
                execlp(argv[2], argv[2], NULL);
                _exit(1);
            }
            wait(&status);
            if (WIFEXITED(status) && !WEXITSTATUS(status)) {
                _exit(1);
            } else {
                _exit(0);
            }
            _exit(1);
        }
        _exit(0);
        close(pfd[1]);
    }
    close(pfd[1]);
    pid_t pid4 = fork();
    if (!pid4) {
        if (dup2(pfd[0], 0) == -1) {
            _exit(1);
        }
        close(pfd[0]);
        int fd = open(argv[5], O_WRONLY | O_CREAT | O_APPEND, 0660); 
        if (fd < 0) {
            _exit(1);
        }
        if (dup2(fd, 1) == -1) {
            _exit(1);
        }
        close(fd); 
        if (!fork()) {
            execlp(argv[3], argv[3], NULL);
            _exit(1);
        }
        close(pfd[1]);
    }
    close(pfd[0]);
    close(pfd[1]);
    while (wait(NULL) > 0) {}
    return 0;
}

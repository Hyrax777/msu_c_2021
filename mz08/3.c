#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>


int mysys(const char *str) {
    int pid = fork();
    if (pid < 0) {
        return 0;
    } else if (!pid) {
        execlp(str,  str, NULL);
        _exit(1);
    }
    int status;
    waitpid(pid, &status, 0);
    return (WIFEXITED(status) && !WEXITSTATUS(status));
}


int main(int argc, char** argv) {
    return !((mysys(argv[1]) || mysys(argv[2])) && mysys(argv[3]));
}

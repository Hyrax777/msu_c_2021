#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>


int  main(int argc, char** argv) {
    int pid = fork();
    if (pid < 0) {
        exit(1);
    } else if (!pid) {
        int fd = open(argv[2], O_RDONLY);
        if (fd < 0) {
            _exit(42);
        }
        if (dup2(fd, 0) == -1) {  // скопировать файловый дескриптор fd в файловый дескриптор 0
            _exit(42);
        } 
        close(fd);   // fd больше не нужен и должен быть закрыт

        fd = open(argv[3], O_WRONLY | O_CREAT | O_APPEND, 0660); 
        if (fd < 0) {
            _exit(42);
        }
        if (dup2(fd, 1) == -1) {    // скопировать файловый дескриптор fd в файловый дескриптор 2
            _exit(42);
        }
        close(fd);   // fd больше не нужен и должен быть закрыт

        fd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0660); 
        if (fd < 0) {
            _exit(42);
        }
        
        if (dup2(fd, 2) == -1) { // скопировать файловый дескриптор fd в файловый дескриптор 1
            _exit(42);
        }
        close(fd);   // fd больше не нужен и должен быть закрыт

        execlp(argv[1], argv[1], NULL);
        _exit(42);
    } else {
        int status = 0;
        wait(&status);
        printf("%d", status);
    }
    return 0;
}

//CMD < FILE1 >> FILE2 2> FILE3
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>


int main(int argc, char** argv) {
    unsigned long long rand;
    int i, pid, f1;
    char *envir;
    char path[PATH_MAX];
    if ((envir = getenv("XDG_RUNTIME_DIR")) == NULL) {
        if (((envir = getenv("TMPDIR")) == NULL)) {
            envir = "/tmp";
        }
    }
    pid = getpid();
    f1 = open("/dev/urandom", O_RDONLY);
    if ((f1 == -1) || (read(f1, &rand, sizeof(rand)) != sizeof(rand))) {
        exit(1);
    } 
    f1 = close(f1);
    if (snprintf(path, PATH_MAX, "%s/scr_%d_%llu.py", envir, pid, rand) < 0) {
        exit(1);
    }
    FILE* fd = fopen(path, "w");
    if (fd == NULL) {
        exit(1);
    }
    chmod(path, S_IRUSR | S_IXUSR);
    if (fprintf(fd, "#! /usr/bin/python3\nimport os\nprint(") < 0) {
        exit(1);
    }
    for (i = 1; i < argc - 1; i++) {
        if (fprintf(fd, "%s*", argv[i]) < 0) {
            exit(1);
        }
    }
    if (fprintf(fd, "%s)\nos.remove(\"%s\")\n", argv[argc - 1], path) < 0) {
        exit(1);
    }
    fclose(fd);
    execlp(path, path, NULL);
    return 0;
}


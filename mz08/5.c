#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>

enum
{
    MAXLEN = 11,
};

int main(int argc, char** argv) {
    char el[MAXLEN];
    unsigned long long rand;
    int pid, f1;
    char *envir;
    char path[PATH_MAX], comppath[PATH_MAX];
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
    if (snprintf(path, PATH_MAX, "%s/src_%d_%llu.c", envir, pid, rand) < 0) {
        exit(1);
    }
    if (snprintf(comppath, PATH_MAX, "%s/src_%d_%llu", envir, pid, rand) < 0) {
        exit(1);
    }
    FILE* fd = fopen(path, "w");
    if (fd == NULL) {
        exit(1);
    }
    chmod(path, 0500);
    fprintf(fd, "#include <stdio.h>\n\
#include <stdlib.h>\n\
\n\
enum\n\
{\n\
    BASE = 10, // система счисления\n\
};\n\
\n\
int main(int argc, char** argv) {\n\
    int reject = 1, summon = 2, disqualify = 3;\n\
    char *end;\n\
    int x = strtol(argv[1], &end, BASE);\n\
    int ans = ");
    fprintf(fd, "%s;\n", argv[1]);
    fprintf(fd,"    if (ans == 1) {\n\
        printf(\"reject\\n\");\n\
    }\n\
    if (ans == 2) {\n\
        printf(\"summon\\n\");\n\
    }\n\
    if (ans == 3) {\n\
        printf(\"disqualify\\n\");\n\
    }\n\
    return 0;\n\
}");
    fclose(fd);
    if ((pid = fork()) == -1) {
        printf("fork err");
        exit(1);
    } else if (pid == 0) {
        execlp("gcc", "gcc", path, "-o", comppath, NULL);
        printf("execlp err");
        _exit(1);
    }
    wait(NULL);
    while (scanf("%s", el) > 0) {
        if ((pid = fork()) == -1) {
            printf("Fork err");
            exit(1);
        } else if (pid == 0) {
            execlp(comppath, comppath, el, NULL);
            printf("Execlp err");
            _exit(1);
        }
        wait(NULL);
    }
    remove(path);
    remove(comppath);
    return 0;
}
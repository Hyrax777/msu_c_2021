#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <dlfcn.h>


enum
{
    BASE = 10, // система счисления   
};


int main(int argc, char** argv) {
    int fd, pid, i;
    double x, s = 0, res = 0;

    int n = strtol(argv[3], NULL, BASE);
    double left = strtod(argv[1], NULL);
    double right = strtod(argv[2], NULL);
    double dx = (right - left) / n;
    fd = open("src.c", O_CREAT | O_TRUNC | O_WRONLY, 0755);
    if (fd == -1) {
        exit(1);
    }
    dprintf(fd, "#include <stdio.h>\n\
#include <math.h>\n\
\n\
double func(double x) {\n\
return ");
    dprintf(fd, "%s;\n}", argv[4]);
    fd = close(fd);

    if ((pid = fork()) == -1) {
        exit(1);
    } else if (pid == 0) {
        execlp("gcc", "gcc", "src.c", "-fPIC", "-lm", "-c", NULL);
        exit(1);
    }
    wait(NULL);
    if ((pid = fork()) == -1) {
        exit(1);
    } else if (pid == 0) {
        execlp("gcc", "gcc", "src.o", "-shared", "-o", "src.so", NULL);
        exit(1);
    }
    wait(NULL);
    void *handle = dlopen("./src.so", RTLD_LAZY);
    void *func = dlsym(handle, "func");
    for (i = 0; i < n; i++) {
        x = left + i * dx; 
        s = ((double (*)(double)) func)(x) * dx;
        res += s;
    }
    dlclose(handle);
    printf("%.10g\n", res);
    remove("src.c");
    remove("src.o");
    remove("src.so");
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <dlfcn.h>

enum
{
    SIZE = 64, //  размер аргументов
    BASE = 10, // система счисления
    ITER = 3, // номер аргументы сигнатуры
};

struct Param
{
    unsigned char arr[SIZE];
};


int main(int argc, char** argv) {
    struct Param forst= {0};
    int i, off = 0, temp1;
    double temp2;
    char ret_type = argv[ITER][0];
    char *end;
    void *handle = dlopen(argv[1], RTLD_LAZY);
    if (!handle) {
        return 1;
    }
    void *func = dlsym(handle, argv[2]);
    int len = strlen(argv[ITER]);
    for (i = 1; i < len; i++) {
        switch(argv[ITER][i]) {
            case 'i':
                temp1 = strtol(argv[i + ITER], &end, BASE);
                memcpy(forst.arr + off, &temp1, sizeof(int));
                off += sizeof(int);
                break;
            case 'd':
                temp2 = strtod(argv[i + ITER], &end);
                memcpy(forst.arr + off, &temp2, sizeof(double));
                off += sizeof(double);
                break;
            case 's':
                memcpy(forst.arr + off, &argv[i + ITER], sizeof(char*));
                off += sizeof(char*);
                break;
            default:
                break;
        }

    }
    if (ret_type == 'v') {
        ((void (*)(struct Param)) func)(forst);
    } else {
        if (ret_type == 'i') {
            int res1 = ((int (*)(struct Param)) func)(forst);
            printf("%d\n", res1);
        }
        if (ret_type == 'd') {
            double res2 = ((double (*)(struct Param)) func)(forst);
            printf("%.10g\n", res2);
        }
        if (ret_type == 's') {
            char *s = ((char* (*)(struct Param)) func)(forst);
            printf("%s\n", s);
        }
    }
    dlclose(handle);
    return 0;
}
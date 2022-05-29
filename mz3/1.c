#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

enum 
{
    SIZE_OF_KIBI = 1024, // количество байт в кибибайте
};

int main(int argc, char** argv) {
    int i, fd;
    long long sum = 0;
    for (i = 1; i < argc; i++) {
        struct stat buf;
        fd = lstat(argv[i], &buf);
        if (!fd && (buf.st_nlink == 1)  && (buf.st_size % SIZE_OF_KIBI == 0) && 
            ((S_ISREG(buf.st_mode) == 1)))
            sum += buf.st_size;
    }
    printf("%lld\n", sum);
    return 0;
}

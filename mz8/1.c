#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

enum
{
    MASK1 = 0x1FF, // последние 9 бит числа
    BIAS = 9, //для первых 7 бит числа
    SIZE = 64 * 1024, // 64 кибибайт
};


int main(int argc, char** argv) {
    unsigned short cur, url, off, curph, urlph, tab, ph;
    if (sscanf(argv[2], "%hx", &tab) < 1) {
        return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd != -1) {
        struct stat buf;
        fstat(fd, &buf);
        if (SIZE != buf.st_size) {
            return 1;
        }
        char* map = mmap(NULL, SIZE, PROT_READ, MAP_SHARED, fd, 0);
        if (map == MAP_FAILED) {
            return 1;
        }
        while (scanf("%hx", &cur) > 0) {
            url = cur >> BIAS;
            off = cur & MASK1;
            urlph = *(short*)&map[tab + 2*url];
            curph = urlph | off;
            ph = *(short*)&map[curph] ;
            printf("%hu\n", ph);
        }
        fd = close(fd);
        munmap(map, SIZE);
    } 
    return 0;
}

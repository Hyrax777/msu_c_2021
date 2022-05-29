#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <stdint.h>


int main(int argc, char** argv) {
    int fd = open(argv[1], O_RDONLY), ok = 0;
    unsigned short cur, min = INT16_MAX;
    unsigned char num[2];
    while (read(fd, num, sizeof(num)) == sizeof(num)) {
        cur = ((unsigned short) num[0] << CHAR_BIT) | (unsigned short) num[1];
        if (cur % 2 == 0) {
            if (cur < min) {
                min = cur;
                ok = 1;
            }
        }
    }
    if (ok) {
        printf("%hu\n", min);
    }
    fd = close(fd);
    return 0;
}
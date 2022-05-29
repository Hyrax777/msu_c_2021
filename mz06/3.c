#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>


int main(int argc, char** argv) {
    int row, col, j, k = 0, cur = 1, res;
    int fd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd == -1) {
        return 1;
    }
    if (sscanf(argv[2], "%d", &row) < 1) {
        return 1;
    }
    if (sscanf(argv[3], "%d", &col) < 1) {
        return 1;
    }
    if (__builtin_mul_overflow(col, row, &res)) {
        return 1;
    }
    if (__builtin_mul_overflow(res, sizeof(row), &res)) {
        return 1;
    }
    ftruncate(fd, res);
    int *map = (int*)mmap(NULL, res, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
            return 1;
    }
    while (cur <= row * col) {
        k++;
        for (j = k - 1; (j < col - k + 1) &&  (cur <= row * col); j++) {
            map[col * (k - 1) + j] = cur++;
        } 
        for (j = k; (j < row - k + 1) && (cur <= row * col); j++) {
            map[col * j + col - k] = cur++;
        }
        if (row != 1) {
            for (j = col - k - 1; (j >= k - 1) && (cur <= row * col); j--) {
                map[col * (row - k) + j] = cur++;
            }
        }
        if (col != 1) {
            for (j = row - k - 1; (j >= k) && (cur <= row * col); j--) {
                map[col * j + k - 1] = cur++;
            }
        }
    }     
    fd = close(fd);
    munmap(map, sizeof(int) * row * col);
    return 0;
}
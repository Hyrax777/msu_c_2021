#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <stdint.h>


struct Node 
{
    int32_t key;
    int32_t left_idx;
    int32_t right_idx;
};

void bypass(int f1, int id) {
    struct Node vertex;
    if (lseek(f1, id * sizeof(vertex), SEEK_SET) == -1) {
        return;
    }
    if (read(f1, &vertex, sizeof(vertex)) != sizeof(vertex)) {
        return;
    }
    if (vertex.right_idx != 0)  {
        bypass(f1, vertex.right_idx);
    }
    printf("%d ", vertex.key);
    if (vertex.left_idx != 0) {
        bypass(f1, vertex.left_idx);
    }
    return;
}

int main(int argc, char** argv) {
    int f1 = open(argv[1], O_RDONLY);
    if (f1 != -1) {
        bypass(f1, 0);
    }
    printf("\n");
    close(f1);
    return 0;
}
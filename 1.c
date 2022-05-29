#include <stdio.h>
#include <stdlib.h>


enum
{
    BASE = 10, //система счисления
};


int main(int argc, char** argv) {
    int miss = 0, addr, size, value, curr_bl;
    char act[2];
    int cache_sz = strtol(argv[2], NULL, BASE);
    int block_sz = strtol(argv[3], NULL, BASE);
    int num = cache_sz / block_sz;
    int* arr = malloc(num * sizeof(num));
    if (arr == NULL) {
        return 1;
    }
    for (int i = 0; i < num; i++) {
        arr[i] = -1;
    }
    while (scanf("%s %x %d %d", act, &addr, &size, &value) > 0) {
        curr_bl = addr / block_sz;
        if (act[0] == 'R') {
            if ((arr[curr_bl % num] != curr_bl) && (arr[curr_bl % num] != -1)) {
                miss++;
            }
        }
        arr[curr_bl % num] = curr_bl;

    }
    printf("%d\n", miss);
    free(arr);
    return 0;
}

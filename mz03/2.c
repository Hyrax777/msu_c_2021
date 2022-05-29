#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

enum
{
    BASE = 8, // система счисления
};

int main(int argc, char** argv) {
    int i, j, cur;
    for (i = 1; i < argc; i++) {
        char patt[] = "rwxrwxrwx";
        enum
        {
            SIZE = sizeof(patt) - 1,
        };
        cur = strtol(argv[i], NULL, BASE);
        for (j = 0; j < SIZE; j++) {
            if (!((cur & (1 << (SIZE - j - 1))) >> (SIZE - j - 1)))
                patt[j] = '-';
        }
        printf("%s\n", patt);
    }
    return 0;
}
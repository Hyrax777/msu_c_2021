#include <stdio.h>
#include <stdlib.h> 

enum
{
    BASE = 10,  // система счисления
};


int main(int argc, char **argv) {
    long long pos = 0, neg = 0, cur;
    for (int i = 1; i < argc; i++) {
        cur = strtol(argv[i], NULL, BASE);
        if (cur > 0) {
            pos += cur;
        } else {
            neg += cur;
        }
    }
    printf("%lld\n%lld\n", pos, neg);
    return 0;
}

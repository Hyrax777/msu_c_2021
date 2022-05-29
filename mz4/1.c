#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

enum 
{
    BASE = 10, //основание системы 
    INS = 1000000, // микросекунд в секунде
};

int main(int argc, char** argv) {
    srand(strtod(argv[5], NULL));
    double u, add;
    struct timeval t0 = {strtol(argv[1], NULL, BASE), strtol(argv[2], NULL, BASE)};
    int n = strtol(argv[4], NULL, BASE);
    for (int i = 0; i < n; i++) {
        u = (double) rand() / RAND_MAX;
        add = floor(-log(u) / strtod(argv[3], NULL));
        t0.tv_usec += add;
        if (t0.tv_usec > INS) {
            t0.tv_sec += t0.tv_usec / INS;
            t0.tv_usec %= INS;
        }
        printf("%ld %ld\n",  t0.tv_sec, t0.tv_usec);
    }
    return 0;
}

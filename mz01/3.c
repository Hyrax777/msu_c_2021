#include <stdio.h>
#include <stdlib.h> 
#include <math.h>

enum
{
    BASE = 100, // 100% для расчета коэффицента
    RATE = 10000, // чтобы оставить 4 значащие цифры после запятой
};


int main(int argc, char **argv) {
    double res = 0, perc;
    char *end;
    if (argc > 1)
        res = strtod(argv[1], &end);
    for (int i = 2; i < argc; i++) {
        perc = (BASE + strtod(argv[i], &end))/BASE; // коэффициент для нового результата
        res = res * perc;
        res = round(res * RATE)/ RATE;  // 
    }    
    printf("%0.4lf\n", res);
    return 0;
}

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>

enum
{
    SIZE = 20, // общее число байт каждой записи
    OFF1 = 0xFF000000, // четвертый байт
    OFF2 = 0xFF0000, // третий байт
    OFF3 = 0xFF00, // второй байт
    BIAS1 = 24, // сдвиг для 4 байта
    BIAS2 = 16, // сдвиг для 3 байта 
    BIAS3 = 8, // сдвиг для 2 байта
    NUM1 = 16, // номер в массиве 1 байта числа
    NUM2 = 17, // номер в массиве 2 байта числа
    NUM3 = 18, // номер в массиве 3 байта числа
    NUM4 = 19, // номер в массиве 4 байта числа
    NUM_KNAT = 29, // 1 сикль == 29 кнат
    NUM_SIK = 17, // 1 галеон == 17 сиклей
};

int main(int argc, char** argv) {
    int i, f1, gal, sik, knot, cur;
    long long maxlong, max1 = INT64_MIN, max2 = INT64_MIN;
    unsigned char arr[SIZE];
    for (i = 1; i < argc; i++) {
        f1 = open(argv[i], O_RDONLY);
        while (read(f1, arr, sizeof(arr)) > 0) {
            cur = (int) (((unsigned int) arr[NUM1] << BIAS1) & OFF1) |
            (((unsigned int) arr[NUM2] << BIAS2) & OFF2) |
            (((unsigned int) arr[NUM3] << BIAS3) & OFF3) | (unsigned int) arr[NUM4];
            if (cur > max1) {
                max2 = max1;
                max1 = cur;
            } else if ((cur != max1) & (cur > max2))
                max2 = cur;
        }
    }
    if ((max2 < 0)  && (max2 != INT64_MIN)) {
        putchar('-');
        maxlong = ~max2 + 1;
    } else {
        maxlong = max2;
    }
    knot = maxlong % NUM_KNAT;
    sik = (maxlong / NUM_KNAT) % NUM_SIK;
    gal = (maxlong / NUM_KNAT) / NUM_SIK;
    if (maxlong != INT64_MIN)
        printf("%dg%.2ds%.2dk\n", gal, sik, knot);
    return 0;
}
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <stdint.h>

enum 
{
    NUM = 7, // общее количество возможных вариантов голосов
    REC_NUM = 251, // максимальное количество записей
    SIZE = 2, // байт в 16 битном числе
};

int main(int argc, char** argv) {
    int f1, i;
    uint16_t num1, num2, rec[REC_NUM] = {0}, sum1 = 0, sum2 = 0, diff = 0;
    long long allvoi[NUM] = {0};
    f1 = open(argv[1], O_RDONLY);
    while (read(f1, &num2, SIZE) > 0) {
        read(f1, &sum2, SIZE);
        if (sum1 != 0) {
            if (sum2 != sum1) {
                diff = sum2 - sum1;
                for (i = 0; i < num1; i++) {
                    if ((rec[i] > 1) && (diff > 0)) {
                        rec[i]++;
                        if (--diff <= 0)
                            break;
                    }
                }
                if (diff > 0) {
                    for (i = 0; i < num1; i++) {
                        if ((rec[i] == 1) && (diff > 0)) {
                            rec[i]++;
                            if (--diff <= 0)
                                break;
                        }
                    }
                }
            }
            for (i = 0; i < num1; i++)
                allvoi[rec[i]]++;
        }
        if (num2 != 0) {
            sum1 = 0;
            num1 = num2;
            read(f1, rec, num2*SIZE);
            for (i = 0; i < num2; i++)
                sum1 += rec[i];
        }
    }
    for (i = 1; i < NUM; i++)
        printf("%lld ", allvoi[i]);
    printf("\n");
    close(f1);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>

enum
{
    A = 1103515245, // мультипликатор
    C = 12345, // инкремент
    MOD = 1ll << 31, // модуль
};

typedef struct RandomGenerator RandomGenerator;

typedef struct RandomOperations
{
    unsigned int (*next)(RandomGenerator *);
    void (*destroy)(RandomGenerator *);
} RandomOperations;


typedef struct RandomGenerator
{
    unsigned int cur;
    const RandomOperations *ops;
} RandomGenerator;

unsigned int next(RandomGenerator *temp) {
    temp->cur = (A * temp->cur + C) % MOD;
    return temp->cur;
}

void destroy(RandomGenerator *temp) {   
    free(temp);
}

static struct RandomOperations rop = {next, destroy};

RandomGenerator *random_create(int seed) {
    RandomGenerator *temp = (RandomGenerator *)calloc(1, sizeof(*temp));
    if (temp == NULL) {
        printf("Error\n");
        return 0;
    }
    temp->ops = &rop;
    temp->cur = seed;
    return temp;
}


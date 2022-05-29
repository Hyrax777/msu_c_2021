#include <stdio.h>
#include <inttypes.h>

int main(int argc, char** argv) {
    int32_t n, w, cur1 = 0;
    uint32_t s, max, cur = 0;
    scanf("%"SCNd32" %"SCNu32" %"SCNd32"",&n, &s, &w);
    if (n != 32) {
        max = (uint32_t)1 << n;
    } else {
        max = ~(uint32_t)0;
    }
    while (cur < max) {
        printf("|%*"SCNo32"|%*"SCNu32"|", (int)w, cur, (int)w, cur);
        if (cur & (uint32_t)1 << (n - 1)) {
            cur1 = cur & ~((uint32_t)1 << (n - 1));
            cur1 = ~cur1 + 1;
            printf("%*"SCNd32"|\n", (int)w, cur1);
        } else {
            printf("%*"SCNd32"|\n", (int)w, cur);
        }
        if (cur > UINT32_MAX - s)
            break;
        cur += s;
    }
    return 0;
}
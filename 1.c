#include <limits.h>

STYPE bit_reverse(STYPE value) {
    UTYPE res = 0,  bit;
    for (UTYPE i = 0; i < sizeof(STYPE) * CHAR_BIT; i++) {
        bit = (value >> i) & 1;
        res = (res << 1) | bit;
    }
    return res;
}
#include <stdio.h>
#include <string.h>

int
parse_rwx_permissions(const char *str) 
{
    if (str == NULL) {
        return -1;
    }
    int ans = 0;
    const static char patt[] = "rwxrwxrwx";
    enum
    {
        LEN1 = sizeof(patt),
    };
    int len2 = strlen(str);
    if (LEN1 != len2 + 1) {
        return -1;
    }
    for (int i = 0; i < len2; i++) {
        if (str[i] == patt[i]) {
            ans = ans | (1 << (len2 - 1 - i));
        } else if (str[i] != '-') {
            return -1;
        }
    }
    return ans;
}

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>

enum 
{
    BASE = 10, //количество цифр
};


int main() {
    char str1[PATH_MAX] = {0}, temp[PATH_MAX];
    int f1, i = 0, dim = 0;
    long long arr[BASE] = {0};
    if (fgets(str1, sizeof(str1), stdin) != NULL) {
        for (i = PATH_MAX - 1; i > 0; i--) {
            if (str1[i] == '\n') {
                str1[i] = '\0';
                if (str1[i-1] == '\r')
                    str1[i-1] = '\0';
                break;
            }
        }
        f1 = open(str1, O_RDONLY);
        if (f1 != -1) {
            while ((dim = read(f1, temp, sizeof(temp))) > 0) {
                for (i = 0; i < dim; i++) {
                    if (isdigit((unsigned char) temp[i])) {
                        arr[temp[i] - '0']++;
                    }
                }
            }
            f1 = close(f1);
        }
    }
    for (i = 0; i < BASE; i++) {
        printf("%d %lld\n", i, arr[i]);
    }
    return 0;
}

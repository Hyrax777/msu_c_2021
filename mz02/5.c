#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>


int main(int argc, char** argv) {
    int f1 = open(argv[1], O_RDWR), i = -1, ok = 0;
    long long temp, min = INT64_MAX, pos = 0;
    while (read(f1, &temp, sizeof(temp)) == sizeof(temp)) {
        i++;
        if(!ok) {
            min = temp;
            ok = 1;
        } else if (temp < min) {
            min = temp;
            pos = i;
        }
    }
    if (ok) {
        if (min != INT64_MIN) {
            min = -min;
        }
        if (lseek(f1, pos * sizeof(pos), SEEK_SET) == -1) {
            return 1;
        }
        if (write(f1, &min, sizeof(min)) < sizeof(min)) {
            return 1;
        }
    }
    close(f1);
    return 0;
}
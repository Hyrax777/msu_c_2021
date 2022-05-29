#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int f1 = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    unsigned char temp2[4];
    unsigned int temp1;
    unsigned short num1, num2;
    if (f1 != -1) {
        while (scanf("%u", &temp1)> 0) {
            num1 = temp1 >> 12;
            num2 = temp1 & 0xFFF;
            temp2[3] = num2 & 0xFF;
            temp2[2] = num2 >> 8;
            temp2[1] = num1 & 0xFF;
            temp2[0] = num1 >> 8;
            if (write(f1, temp2, sizeof(temp2)) != sizeof(temp2))
                return 1;
        }
        close(f1);
    }
    return 0;
}
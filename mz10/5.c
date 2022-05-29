// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <unistd.h>
// #include <signal.h>
// #include <limits.h>
// #include <string.h>

// volatile int count_outp = 0, temp = -1;

// enum
// {
//     STOP = 3, // завершение
// };


// void
// output() {
//     static int counter = 0;
//     if (temp == -1) {
//         counter++;
//         printf("0\n"); fflush(stdout);
//     } else {
//         printf("%d\n", temp); fflush(stdout);
//         counter++;
//     }
//     if (counter == STOP) {
//         exit(0);
//     }
// }


// void 
// handler1(int s) 
// {
//     count_outp++;
// }

// void 
// handler2(int s) 
// {
//     exit(0);
// }


// int main() {
//     int low, high, i, j, ok = 1;
//     sigaction(SIGINT, &(struct sigaction) { .sa_handler = handler1, .sa_flags = SA_RESTART }, NULL);
//     sigaction(SIGTERM, &(struct sigaction) { .sa_handler = handler2, .sa_flags = SA_RESTART }, NULL);
//     pid_t pid = getpid();
//     printf("%d\n", pid); fflush(stdout);
//     if (scanf("%d %d", &low, &high) <= 0) {
//         return 1;
//     }
//     for (i = low; i < high; i++) {
//         for (j = 2; j * j < i; j++) {
//             if (i % j == 0) {
//                 ok = 0;
//                 break;
//             }
//         }
//         if (ok) {
//             temp = i;
//         }
//         while (count_outp) {
//             output();
//             count_outp--;
//         }
//         ok = 1;
//     }
//     printf("-1\n"); fflush(stdout);
//     exit(0);
// }



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <limits.h>
#include <string.h>

volatile int counter = 0, temp = -1;

enum
{
    STOP = 3, // завершение
};


void 
handler1(int s) 
{
    if (temp == -1) {
        counter++;
        printf("0\n"); fflush(stdout);
    } else {
        printf("%d\n", temp); fflush(stdout);
        counter++;
    }
    if (counter == STOP) {
        exit(0);
    }
}

void 
handler2(int s) 
{
    exit(0);
}


int main() {
    int low, high, i, j, ok = 1;
    sigaction(SIGINT, &(struct sigaction) { .sa_handler = handler1, .sa_flags = SA_RESTART }, NULL);
    sigaction(SIGTERM, &(struct sigaction) { .sa_handler = handler2, .sa_flags = SA_RESTART }, NULL);
    pid_t pid = getpid();
    printf("%d\n", pid); fflush(stdout);
    if (scanf("%d %d", &low, &high) <= 0) {
        return 1;
    }
    for (i = low; i < high; i++) {
        for (j = 2; j * j < i; j++) {
            if (i % j == 0) {
                ok = 0;
                break;
            }
        }
        if (ok) {
            temp = i;
        }
        ok = 1;
    }
    printf("-1\n"); fflush(stdout);
    exit(0);
}
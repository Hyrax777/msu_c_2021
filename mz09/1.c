#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>


enum
{
    START_YEAR = 1900, // год начала отсчета
};

int main() {
    int fds[2];
    pipe(fds);
    int pid = fork();
    if (pid < 0) {
        exit(1);
    } else if (!pid) {
        pid = fork();
        if (pid < 0) {
            exit(1);
        } else if (!pid) {
            pid = fork();
            if (pid < 0) {
                exit(1);
            } else if (!pid) {
                pid = fork();
                if (pid < 0) {
                    exit(1);
                } else if (!pid) {
                    close(fds[0]);
                    time_t cur;
                    time(&cur);
                    write(fds[1], &cur, sizeof(cur)); 
                    write(fds[1], &cur, sizeof(cur)); 
                    write(fds[1], &cur, sizeof(cur)); 
                    close(fds[0]);
                } else if (pid > 0) {
                    close(fds[1]);
                    wait(NULL);
                    time_t cur;
                    read(fds[0], &cur, sizeof(cur));
                    struct tm temp;
                    localtime_r(&cur, &temp);
                    printf("D:%2d\n", temp.tm_mday);
                    fflush(stdout);              
                }
            } else if (pid > 0) {
                close(fds[1]);
                wait(NULL);
                time_t cur;
                read(fds[0], &cur, sizeof(cur));
                struct tm temp;
                localtime_r(&cur, &temp);
                printf("M:%2d\n", temp.tm_mon + 1);   
                fflush(stdout);               
            }
        } else if (pid > 0) {
            close(fds[1]);
            wait(NULL);
            time_t cur;
            read(fds[0], &cur, sizeof(cur));
            struct tm temp;
            localtime_r(&cur, &temp);
            printf("Y:%4d\n", START_YEAR + temp.tm_year);   
            fflush(stdout); 
        }
    }
    wait(NULL);
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>

enum
{
    BASE = 10, //система счисления
};

void 
work(int serial, int semid, int iterct, int count, int *arr) {
    for (int i = 0; i < iterct; i++) {
        int i1 = rand() % count;
        int i2 = rand() % count;
        int val = rand() % BASE;
        if(i1 != i2) {
            struct sembuf up[] =
            {
                { .sem_num = i1, -1, SEM_UNDO },
                { .sem_num = i2, -1, SEM_UNDO },
            };
            int flg = semop(semid, up, sizeof(up) / sizeof(up[0]));
            if (flg < 0) {
                exit(1);
            }
            operation(arr, i1, i2, val);
            struct sembuf down[] =
            {
                { .sem_num = i1, 1, SEM_UNDO },
                { .sem_num = i2, 1, SEM_UNDO },
            };
            flg = semop(semid, down, sizeof(down) / sizeof(down[0]));
            if (flg < 0) {
                exit(1);
            }

        }
    }
}




int main(int argc, char** argv) {
    int pid, cur;
    int count = strtol(argv[1], NULL, BASE);
    int key = strtol(argv[2], NULL, BASE);
    int n_proc = strtol(argv[3], NULL, BASE);
    int iterct = strtol(argv[4], NULL, BASE);
    int semid = semget(key,  count, IPC_CREAT | IPC_EXCL | 0600);
    if (semid < 0) {
        exit(1);
    }
    short *buf = calloc(1, sizeof(buf[0]));  // from git
    for (int i = 0; i < count; ++i) {
        buf[i] = 1;
    }
    semctl(semid, 0, SETALL, buf);
    free(buf);
    int *arr = mmap(NULL, count * sizeof(pid), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    for (int i = 0; i < n_proc; i++) {
        cur = strtol(argv[i + 5], NULL, BASE);
        pid = fork();
        if (pid < 0) {
            semctl(semid, 0, IPC_RMID);
            exit(1);
        } else if (!pid) {
            srand(cur);
            work(i, semid, iterct, count, arr);
            exit(0);
        }
    }
    while (wait(NULL) > 0) {}
    semctl(semid, 0, IPC_RMID);
    munmap(arr, 0);
    return 0;
}

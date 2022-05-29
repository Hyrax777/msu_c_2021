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
work(int serial, int semid, int *buf1, int *buf2, int n_proc, int stop) {
    while (1) {
        struct sembuf down[] =
        {
            { .sem_num = serial, -1, 0},
        };
        int flg = semop(semid, down, sizeof(down) / sizeof(down[0]));
        if (flg < 0) {
            if (errno == EIDRM) {
                exit(0);
            }
            semctl(semid, 0, IPC_RMID);
            exit(1);
        }
        printf("%d %d %d\n", serial, *buf1, *buf2); fflush(stdout);
        int val = buf1[0];
        val++;
        if ((*buf1) >= stop) {
            semctl(semid, 0, IPC_RMID);
            exit(0);
        }
        buf1[0] = val;
        long long next = val % n_proc;
        next = (next * next) % n_proc;
        next = (next * next) % n_proc;
        next = next + 1;
        *buf2 = serial;
        struct sembuf up[] =
        {
            { .sem_num = next, 1, 0 },
        };
        flg = semop(semid, up, sizeof(up) / sizeof(up[0]));
        if (flg < 0) {
            if (errno == EIDRM) {
                exit(0);
            }
            semctl(semid, 0, IPC_RMID);
            exit(1);
        }
    }   
}


int main(int argc, char** argv) {
    int pid;
    int n_proc = strtol(argv[1], NULL, BASE);
    int key = strtol(argv[2], NULL, BASE);
    int maxval = strtol(argv[3], NULL, BASE);
    int semid = semget(key, n_proc + 1, IPC_CREAT | IPC_EXCL | 0600);
    if (semid < 0) {
        exit(1);
    }
    int *buf1 = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    int *buf2 = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
    if ((buf1 == MAP_FAILED) || (buf2 == MAP_FAILED)) {
        semctl(semid, 0, IPC_RMID);
        exit(1);
    }
    for (int i  = 1; i <= n_proc; i++) {
        pid = fork();
        if (pid < 0) {
            semctl(semid, 0, IPC_RMID);
            exit(0);
        }
        if (!pid) {
            work(i, semid, buf1, buf2, n_proc, maxval);
            exit(0);
        }
    }
    *buf1 = 0;
    struct sembuf up[] =
    {
        { .sem_num = 1, 1, SEM_UNDO },
    };
    int flg = semop(semid, up, sizeof(up) / sizeof(up[0]));
    if (flg < 0) {
        if (errno == EIDRM) {
            exit(0);
        }
        semctl(semid, 0, IPC_RMID);
        exit(1);
    }
    while (wait(NULL) > 0) {}
    semctl(semid, 0, IPC_RMID);
    munmap(buf1, 0);
    munmap(buf2, 0);
    return 0;
}

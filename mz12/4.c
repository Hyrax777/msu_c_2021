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
work(int serial, int semid, int count) {
    while (1) {
        struct sembuf down[] =
        {
            { .sem_num = serial, -1, SEM_UNDO },
        };
        int flg = semop(semid, down, sizeof(down) / sizeof(down[0]));
        if (flg < 0) {
            if (errno == EIDRM) {
                exit(0);
            }
            exit(1);
        }
        int temp;
        if (scanf("%d", &temp) != 1) {
            semctl(semid, 0, IPC_RMID);
            exit(0);
        }
        printf("%d %d\n", serial, temp); fflush(stdout);
        int new_id = (temp % count) + count;
        new_id = new_id % count;
        struct sembuf up[] =
        {
            { .sem_num = new_id, 1, SEM_UNDO },
        };
        flg = semop(semid, up, sizeof(up) / sizeof(up[0]));
        if (flg < 0) {
            if (errno == EIDRM) {
                exit(0);
            }
            exit(1);
        }
    }
}


int main(int argc, char** argv) {
    int pid, key = 36;
    int count = strtol(argv[1], NULL, BASE);
    int semid = semget(key, count, IPC_CREAT | IPC_EXCL | 0600);
    if (semid < 0) {
        exit(1);
    }
    for (int i = 0; i < count; i++) {
        pid = fork();
        if (pid < 0) {
            semctl(semid, 0, IPC_RMID);
            exit(1);
        } else if (!pid) {
            setbuf(stdin, NULL);
            work(i, semid, count);
            exit(0);
        }
    }
    struct sembuf up[] =
    {
        { .sem_num = 0, 1, SEM_UNDO },
    };
    int flg = semop(semid, up, sizeof(up) / sizeof(up[0]));
    if (flg < 0) {
        if (errno == EIDRM) {
            exit(0);
        }
        exit(1);
    }
    while (wait(NULL) > 0) {}
    semctl(semid, 0, IPC_RMID);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>

int main(){
    int key = 15;
    int semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0600);
    if (semid < 0) {
        printf("%s\n", strerror(errno));
        exit(1);
    }

    //set all 1
    short *ptr = calloc(1, sizeof(ptr[0]));
    for (int i = 0; i < 1; ++i) ptr[i] = 1;
    semctl(semid, 0, SETALL, ptr);
    free(ptr);

    struct sembuf b[] = {
         { .sem_num = 0, -1, SEM_UNDO }
    };
    semop(semid, b, sizeof(b)/sizeof(b[0]));

    semctl(semid, 1, 0);
    return 0;
}
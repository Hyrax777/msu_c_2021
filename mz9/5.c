#include <stdio.h>
#include <stdlib.h>
#include <sys/eventfd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

// serial - порядковый номер процесса-сына (1, 2)
// rfd - для ожидания своей очереди вывода
// wfd - для уведомления другого процесса
void work(int serial, int rfd, int wfd, long long stop) {
    while (1) {
        // сами данные нас не интересуют, важно, чтобы был фиксированный
        // размер, в нашем случае берем sizeof(int)
        uint64_t val;
        // ожидаем своей очереди для работы
        fscanf(rfd, "%lld", val);
        // выводим свой порядковый номер
        printf("%d %lld\n", serial, val++); fflush(stdout);
        if (val >= stop) {
            close(rfd);
            close(wfd);
            _exit(0);
        }
        // разрешаем работать другому процессу
        fprintf(wfd, "%lld", val);
    }
}

int main(int argc, char** argv) {
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    long long stop;
    stop = strtol(argv[1], NULL, 10);
    //int p1 = eventfd(1, 0);  // начальное значение 1, стандарный режим работы
    //int p2 = eventfd(0, 0);  // начальное значение 0, стандарный режим работы
    FILE *rf = fdopen(pfd[0], "r");
    FILE *wf = fdopen(pfd[1], "w");
    if (!fork()) {
        // первый сын
        work(1, p1[0], p2[1], stop);
        _exit(0);
    }
    if (!fork()) {
        // второй сын
        work(2, p2[0], p1[1], stop);
        _exit(0);
    }
    // ждем завершения всех сыновей, чего нормально не должно произойти никогда

    while (wait(NULL) > 0) {}

    printf("Done\n");
    return 0;
}
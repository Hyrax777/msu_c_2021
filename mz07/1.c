#include <fcntl.h>
#include <unistd.h>

int proc(void)
{
    int pid = fork();
    if (!pid) {
        write(1, "1\n", 2);
    }
    return pid;
}


int main() {
    proc(), proc(), proc();
    return 0;
}

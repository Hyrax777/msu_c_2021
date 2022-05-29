#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

enum
{
    OFF1 = 0x1C0, // 3 бита владельца
    OFF2 = 0x38,  // 3 бита группы
    OFF3 = 0x7,  // 3 бита остальных пользователей
    BIAS1 = 6, // смещение 3 бит владельца
    BIAS2 = 3,  // смещение 3 бит группы
};

struct Task
{
    unsigned uid;
    int gid_count;
    unsigned *gids;
};

static int 
comp(int a, int b) 
{
    return ((a & b) == b);
}

int 
myaccess(const struct stat *stb, const struct Task *task, int access) 
{
    if (task->uid == 0)
        return 1;
    if (task->uid == stb->st_uid)
        return comp((stb->st_mode & OFF1) >> BIAS1, access);
    for (int i = 0; i < task->gid_count; i++) {
        if (stb->st_gid == task->gids[i])
            return comp((stb->st_mode & OFF2) >> BIAS2, access);
    }
    return comp((stb->st_mode & OFF3), access);   
}

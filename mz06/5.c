#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int my_cmp(const void * a,const void *b) {
    return strcasecmp((char *) a, (char *) b);
}

void dfs(char *str) {
    DIR* dir = opendir(str);
    if (dir == NULL) {
        return;
    }
    struct dirent *cur;
    struct stat buf;
    int i = 0, n = 0, fd;
    char *store = NULL;
    while ((cur = readdir(dir)) != NULL) {
        char fullp[PATH_MAX] = {0};
        if ((snprintf(fullp, sizeof(fullp), "%s/%s", str, cur->d_name)) < sizeof(fullp)) {
            fd = lstat(fullp, &buf);
            if ((fd != -1)  && (S_ISDIR(buf.st_mode)) && !access(fullp, R_OK) 
                        && (strcmp(cur->d_name, ".")  && strcmp(cur->d_name, ".."))) {
                store = (char*)realloc(store, NAME_MAX * (++n));
                memcpy(store + NAME_MAX * (n - 1), cur->d_name,  NAME_MAX);
            }
        }
    }
    if (n > 1) {
        qsort(store, n,  NAME_MAX, my_cmp);
    }
    closedir(dir);
    for (i = 0; i < n; i++) {
        char fullp[PATH_MAX] = {0};
        if (snprintf(fullp, sizeof(fullp), "%s/%s", str, store + i * NAME_MAX) < sizeof(fullp)) {
            DIR* dir1 = opendir(str);
            if (dir1 != NULL) {
                printf("cd %s\n", store + i * NAME_MAX);
                closedir(dir1);
                dfs(fullp);
                printf("cd ..\n");
            }
        }
    }
    free(store);
}


int main(int argc, char** argv) {
    dfs(argv[1]);
    return 0;
}
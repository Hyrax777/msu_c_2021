#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc, char** argv) {
    DIR* dir = opendir(argv[1]);
    struct dirent *cur;
    struct stat buf;
    char patt[] = ".exe";
    enum
    {
        SIZE = sizeof(patt) - 1,
    };
    int len, fd;
    long long num = 0;
    if (dir != NULL) {
        while ((cur = readdir(dir)) != NULL) {
            char str[PATH_MAX] = {0};
            if ((snprintf(str, sizeof(str), "%s/%s", argv[1], cur->d_name)) < sizeof(str)) {
                len = strlen(str);
                fd = stat(str, &buf);
                if (len - SIZE >=0) {
                    if (!fd && (S_ISREG(buf.st_mode) == 1) && !(access(str, X_OK)) 
                                && (!strcmp(str + len - SIZE, ".exe"))) {
                        num++;
                    }
                }
            }
        }
        closedir (dir);
    }
    printf("%lld\n", num);
    return 0;
}
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

enum 
{
    START = 1900, // год начала отсчета
    WEEK = 7, // 7 дней в неделе
    NUM = 11, // месяцы от 0 до 11
    OFF = 4, // смещение до ближайшего четверга
};

int main() {
    int year;
    scanf("%d", &year);
    struct tm cur;
    cur.tm_sec = 0; 
    cur.tm_min = 0; 
    cur.tm_hour = 0; 
    cur.tm_isdst = -1;
    cur.tm_mday = 1; 
    cur.tm_mon = 0; 
    cur.tm_year = year - START; 
    mktime(&cur);
    cur.tm_mday += OFF - cur.tm_wday;
    int ok = 0;
    mktime(&cur);
    if (cur.tm_year < year - START)
            cur.tm_mday += WEEK;
    mktime(&cur);
    for (int i = 0; i <= NUM; i++) {
        while (cur.tm_mon == i) {
            if ((cur.tm_mday % 3 != 0) && (ok % 2 != 0))
                printf("%d %d\n", i + 1, cur.tm_mday);
            cur.tm_mday += WEEK;
            mktime(&cur);
            ok++;
        }
        ok = 0;
    }
    return 0;
}
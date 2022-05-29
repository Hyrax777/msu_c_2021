#include <stdio.h>
#include <inttypes.h>
#include <limits.h>


//ПОСМОТРИТЕ ПОЖАЛУЙСТА НЕ ЭТУ ПОСЫЛКУ, А НА 1 НАЗАД
//Я ПРОСТО ХОТЕЛ ИСПРАВИТЬ ENUM И ЗАБЫЛ О PENALTY


enum 
{
    MAX_VAL = 62, // макс сдвиг
    MAX_NUM = 8, // макс число для переполнения
};

int64_t min(int64_t a, int64_t b) {
    return (a < b ? a : b);
}

int64_t calc_max_size(int64_t block_size, int64_t block_num_size, int64_t inode_direct_block_count) {
    int64_t res1 = 0, res2 = 0, temp;
    if (MAX_NUM * block_num_size > MAX_VAL) {
        res1 = -1;
    } else {
        res1 = 1ll << (MAX_NUM * block_num_size);
        if (res1 > INT64_MAX / block_size) {
            res1 = -1;
        } else {
            res1 *= block_size;
        }
    }


    int64_t num = block_size / block_num_size;  
    if (inode_direct_block_count > INT64_MAX - num) {
        res2 = -1;
    } else {
        res2 = inode_direct_block_count + num;
        if (num > INT64_MAX / num) {
            res2 = -1;
        } else {
            temp = num * num;
            if (res2 > INT64_MAX - temp) {
                res2 = -1;
            } else {
                res2 += temp;
                if (temp > INT64_MAX / num) {
                    res2 = -1;
                } else {
                    temp*= num;
                    if (res2 > INT64_MAX - temp) {
                        res2 = -1;
                    } else {
                        res2 += temp;
                        if (res2 > INT64_MAX / block_size) {
                            res2 = -1;
                        } else {
                            res2 *= block_size;
                        }
                    }
                }
            }
        }
    }
    if ((res1 == -1) && (res2 == -1)) {
        return -1;
    }
    if (res1 == -1) {
        return res2;
    } else if (res2 == -1) {
        return res1;
    } else {
        return min(res1, res2);
    }
}



int main() {
    int64_t bl_sz, bl_num_sz, in_dir_bl_ct, num_max = 0, cur, max = -1, i;
    scanf("%"SCNd64" %"SCNd64" %"SCNd64"",&bl_sz, &bl_num_sz, &in_dir_bl_ct); 
    printf("%"PRId64"\n", calc_max_size(bl_sz, bl_num_sz, in_dir_bl_ct));
    for (i = 1; i < MAX_NUM; i++) {
        cur = calc_max_size(bl_sz, i, in_dir_bl_ct);
        if (cur == -1) {
            printf("%"PRId64" -1\n", i);
            return 0;
        }
        if (cur > max) {
            max = cur;
            num_max = i;
        }
    }
    printf("%"PRId64" %"PRId64"\n", num_max, max);
    return 0;
}
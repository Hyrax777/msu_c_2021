#include <stdio.h>
#include <stdlib.h>

int max(int a, int b) {
    return (a > b ? a : b);
}

double ***transpose(double ***arr) {
    int rows, i = 0, j = 0, cols_max = 0, ok = 1;
    while (arr[i++] != NULL) {}
    rows = --i;
    if (rows == 0) {
        double ***res = (double***)calloc(1, sizeof(*res));
        return res;
    }
    int *cur_len = (int*)calloc(rows, sizeof(rows)); 
    while (arr[rows - 1][j++] != NULL) {}
    cur_len[rows - 1] = --j;
    double ***temp = (double***)calloc(rows + 1, sizeof(*temp));
    temp[rows - 1] = (double**)calloc(cur_len[rows - 1], sizeof(**temp));
    for (i = rows - 2; i >= 0; i--) {
        j = 0;
        while (arr[i][j++]) {}
        cur_len[i] = max(--j, cur_len[i + 1]);
        temp[i] = (double**)calloc(cur_len[i], sizeof(**temp));
    }
    cols_max = cur_len[0];
    for (i = 0; i < rows; i++) {
        ok = 1;
        for (j = 0; j < cur_len[i]; j++) {
            if ((ok) && (arr[i][j] == 0)) {
                ok = 0;
            }
            temp[i][j] = (double*)calloc(1, sizeof(double));
            if (ok) {
                temp[i][j][0] = arr[i][j][0];
            }
        }
    }
    double ***res = (double***)calloc(cols_max + 1, sizeof(*res));
    for (i = 0; i < cols_max; i++) {
        res[i] = (double**)calloc(rows + 1, sizeof(**res));
    }
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cur_len[i]; j++) {
            res[j][i] = temp[i][j];
        }
    }
    for (i = 0; i < rows; i++) {
        free(temp[i]);
    }
    free(temp);
    free(cur_len);
    return res;
}


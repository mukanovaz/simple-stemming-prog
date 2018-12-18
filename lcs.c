#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lcs.h"

static int **lcs_matrix;
static int lcs_matrix_rows = 0;
static int lcs_matrix_collumns = 0;

/*----------------------------------
 * Inicializace matici
 *----------------------------------*/
void init_lcs_matrix(int a_length, int b_length){
    if (a_length+1 > lcs_matrix_rows || b_length+1 > lcs_matrix_collumns){
        destroy_lcs_matrix();

        // allocace matici
        lcs_matrix = (int **)malloc((a_length+1) * sizeof(int*));
        for (int i = 0; i < a_length+1; i++)
            lcs_matrix[i] = (int *)malloc((b_length+1) * sizeof(int));

        lcs_matrix_rows = a_length+1;
        lcs_matrix_collumns = b_length+1;
    }

    for (int i = 0; i <= a_length; i++)
        lcs_matrix[i][b_length] = 0;
    for (int j = 0; j <= b_length; j++)
        lcs_matrix[a_length][j] = 0;
}

/*----------------------------------
 * LCS (longest common substring) algoritmus
 *----------------------------------*/
int LCS_algorithm(char *a, char *b, char **longest_common_substring){
    int a_length = strlen(a);
    int b_length = strlen(b);

    init_lcs_matrix(a_length, b_length);

    int max_len = 0, max_index_i = -1;
    for (int i = a_length-1; i >= 0; i--){
        for (int j = b_length-1; j >= 0; j--){
            if (a[i] != b[j]){
                lcs_matrix[i][j] = 0;
                continue;
            }

            lcs_matrix[i][j] = lcs_matrix[i+1][j+1] + 1;
            if (lcs_matrix[i][j] > max_len){
                max_len = lcs_matrix[i][j];
                max_index_i = i;
            }
        }
    }

    if (longest_common_substring != NULL){
        *longest_common_substring = malloc(sizeof(char) * (max_len+1));
        strncpy(*longest_common_substring, a+max_index_i, max_len);
        (*longest_common_substring)[max_len] = '\0';

    }

    return max_len;
}

/*----------------------------------
 * Uvolneni matici
 *----------------------------------*/
void destroy_lcs_matrix () {
    for (int i = 0; i < lcs_matrix_rows; i++)
        free(lcs_matrix[i]);
    free(lcs_matrix);
}
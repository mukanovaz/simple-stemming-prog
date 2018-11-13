#include <stdio.h>
#include "lcs.h"

void find_lcs(char *X, char *Y, int XLen, int YLen, int min_delka_korene, Trie* trie) {
    int L[XLen + 1][YLen + 1];
    int r, c, i;

    // LCS - length
    for(r = 0; r <= XLen; r++) {
        for(c = 0; c <= YLen; c++) {
            if(r == 0 || c == 0) {
                L[r][c] = 0;
            } else if(X[r - 1] == Y[c - 1]) {
                L[r][c] = L[r - 1][c - 1] + 1;
            } else {
                L[r][c] = max(L[r - 1][c], L[r][c - 1]);
            }
        }
    }

    r = XLen;
    c = YLen;
    i = L[r][c];

    char LCS[i+1];

    /*
     * setting the NULL character at the end of LCS character array.
     * as we know in C programming language, NULL character is used
     * to denote the end of a string
     */
    LCS[i] = '\0';

    while(r > 0 && c > 0) {

        if(X[r - 1] == Y[c - 1]) {
            LCS[i - 1] = X[r - 1];
            i--;
            r--;
            c--;
        } else if(L[r - 1][c] > L[r][c - 1]) {
            r--;
        } else {
            c--;
        }

    }

//    printf("Length of the LCS: %d\n", L[XLen][YLen]);
//    printf("LCS: %s\n", LCS);
    // Add LCS word to TRIE
    insert(trie, LCS);
}

int max(int a, int b) {
    return (a > b)? a : b;
}
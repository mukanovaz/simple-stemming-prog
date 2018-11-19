#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lcs.h"


/*
 * Return 1 if s1 and s2 match for the first len chars, 0 otherwise.
 *
 * This could be strcmp(), except that strcmp() returns 0 on a match
 * and -1 or 1 for other cases, and I wanted to write my own.
 */
int prefix_match(char *s1, char *s2, int len)
{
    int i;

    /* A match longer than s1 or s1 doesn't exist. */
    if (len > MIN(strlen(s1), strlen(s2))) {
        return 0;
    }

    for (i = 0; i < len; i++) {
        if (s1[i] != s2[i]) {
            return 0;
        }
    }
    return 1;
}


/* Return the longest common substring of a and b in a freshly allocated string. */

char *longest_common_substring(char *a, char *b)
{
    char *lcs_so_far = a;
    int lcs_length = 0;
    int i, j, match_len;
    char *result;

    for (i = 0; i < strlen(a); i++) {
        for (j = 0; j < strlen(b); j++) {
            match_len = 1;
            while (prefix_match(&(a[i]), &(b[j]), match_len) && a[i] && b[i]) {
                match_len++;
            }                              // Exiting, match_len is one more than the match length
            match_len--;

            if (match_len > lcs_length) {
                lcs_so_far = &(a[i]);
                lcs_length = match_len;
            }
        }
    }
    result = malloc(lcs_length + 1);
    strncpy(result, lcs_so_far, lcs_length);
    result[lcs_length] = '\0';
    return result;
}

#ifndef SP_PC_2018_LCS_H
#define SP_PC_2018_LCS_H

#include "trie.h"

#define MIN(a,b) ((a) < (b) ? (a) : (b))

int prefix_match(char *s1, char *s2, int len);
char *longest_common_substring(char *a, char *b);

#endif //SP_PC_2018_LCS_H

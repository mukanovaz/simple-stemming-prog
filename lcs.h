#ifndef SP_PC_2018_LCS_H
#define SP_PC_2018_LCS_H

#include "trie.h"

int LCS_algorithm(char *s1, char *s2, char **longest_common_substring);
void init_lcs_matrix(int s1_length, int s2_length);
void destroy_lcs_matrix ();

#endif //SP_PC_2018_LCS_H

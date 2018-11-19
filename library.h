#ifndef SP_PC_2018_LIBRARY_H
#define SP_PC_2018_LIBRARY_H

/* ========================================= CONSTANTS ========================================= */
#include "trie.h"

const int  IMPLICITNI_MIN_DELKA_KORENE = 3;
const int  IMPLICITNI_MIN_POCET_VYSKYTU_KORENE = 10;
//const char *DICTIONARY = "output/dictionary.txt";
//const char *STEMS = "output/stems.dat";

const char *DICTIONARY = "/home/janelle/Projects/kiv-pc/output/dictionary.txt";
const char *STEMS = "/home/janelle/Projects/kiv-pc/output/stems.dat";

/* ========================================= FUNCTIONS ========================================= */
void using();
int match(char *string, char *pattern);
void create_dictionary(char *corpus_file_path);
void create_stems_file(int min_delka_korene);
void find_stems(char *word_sequence, int msf_value);
void create_words_array();
void compare_strings(FILE *fp, Trie* trie, List *head, int max_delka_slova);
int get_parameter_value(char *string);
void create_stems_dictionary(const char *filename);
int numbers_only(const char *s);
void create_array(List *head, Word* root, char str[], int level);

#endif //SP_PC_2018_LIBRARY_H

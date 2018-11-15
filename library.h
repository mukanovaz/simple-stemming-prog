#ifndef SP_PC_2018_LIBRARY_H
#define SP_PC_2018_LIBRARY_H

/* ========================================= CONSTANTS ========================================= */
#define ASCI_VAL_0 48
#define ASCI_VAL_9 57

const char*  IMPLICITNI_PRIPONA = ".txt";
const int  IMPLICITNI_MIN_DELKA_KORENE = 3;
const int  IMPLICITNI_MIN_POCET_VYSKYTU_KORENE = 10;

/* ========================================= FUNCTIONS ========================================= */
void using();
int match(char *string, char *pattern);
int get_parameter_value(char *string);
void create_dictionary(char *corpus_file_path);
void create_stems_dictionary(int min_delka_korene);
void find_stems(char *word_sequence, int msf_value);
void create_words_array();
void create_test_trie();

#endif //SP_PC_2018_LIBRARY_H

#ifndef SP_PC_2018_TRIE_H
#define SP_PC_2018_TRIE_H

#include <stdio.h>
#include "LinkedList.h"
/* ========================================= CONSTANTS ========================================= */
#define ALPHA_SIZE 256

/* ========================================= TYPEDEFS ==========================================*/
typedef struct TrieNode Word;
typedef struct TrieStruct Trie;

/* ========================================= STRUCTURES ======================================== */

// Trie ADS
struct TrieStruct {
    Word *root;
    int count;  // count of all nodes in trie
};

// A Trie node
struct TrieNode
{
    int count;  // count of same nodes
    int isLeaf; /* 0 - Leaf;    NON 0 - no Leaf */
    Word* character[ALPHA_SIZE];
};

/* ========================================= FUNCTIONS ========================================= */
Trie* trie_initialize();
Word* get_new_trie_node();
void insert(Trie* trie, char* str);
void display_trie(FILE *fp, Word *root, char prefix[]);
void trie_free (Word *root);
void free_t (Trie* trie);
int is_leaf(Word* node);
void str_clean_cz (char* src);
void str_clean_eng (char* src);
Word* search(Trie* trie, char* str);
int count_characters (const char *word);
void find_stem(Word* root, char *word, char prefix[], int msf_value, char *MAX_STR, size_t stemSize);

char *find_longest_word (char *str);
#endif //SP_PC_2018_TRIE_H

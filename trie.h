#ifndef SP_PC_2018_TRIE_H
#define SP_PC_2018_TRIE_H

#include <bits/types/FILE.h>
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
struct TrieNode* get_new_trie_node();
void insert(Trie* trie, char* str);
Word* search(Trie* trie, char* str);
void trie_free (Word* root);
void free_t (Trie* trie);
int is_leaf(Word* node);
void display(FILE *fp, Word* root, char str[], int level);
Trie* trie_initialize();
void lower_string(char *s);
void str_clean (char* src);
void cz_key_map(int cz_chars[15]);

#endif //SP_PC_2018_TRIE_H

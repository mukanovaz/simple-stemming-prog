#include "trie.h"
#include "lcs.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <regex.h>

Trie* trie_initialize ()
{
    Trie *trie = (Trie*) malloc (sizeof(Trie));
    if (trie == NULL)
    {
        perror("ERROR: in create new trie");
        return NULL;
    }

    trie->count = 0;
    trie->root = get_new_trie_node();
    return trie;
}

// Function that returns a new Trie node
Word* get_new_trie_node()
{
    int i;

    Word* node = (Word*)malloc(sizeof(Word));
    if (node == NULL)
    {
        perror("ERROR: in create new node");
        return NULL;
    }

    node->isLeaf = 0;

    for (i = 0; i < ALPHA_SIZE; i++)
        node->character[i] = NULL;

    return node;
}

void insert(Trie* trie, char* str) {
    Word *curr;
    int index;

    curr = trie->root;

    if (strlen(str) < 3) {
        return;
    }

//    str_clean_cz(str);
    str_clean_eng(str);

    while (*str) {
        index = (unsigned char) *str;
        // create a new node if path doesn't exists
        if (curr->character[index] == NULL) {
            curr->character[index] = get_new_trie_node();
        }
        // go to next node
        curr = curr->character[index];
        // move to next character
        str++;
    }

    trie->count++;
    curr->count++;
    curr->isLeaf = trie->count; // leaf == (non zero)
}

void display_trie(FILE *fp, Word *root, char prefix[]) {
    int i;
    char tmp[strlen(prefix) + 2];

    if (!root) {
        return;
    }

    // If node is leaf node -> display
    if (!is_leaf(root))
    {
        //printf("%s %d\n", prefix, root->count);
        fprintf(fp, "%s %d\n", prefix, root->count);
    }

    for (i = 0; i < ALPHA_SIZE; i++)
    {
        // if we found NOT NULL child, write it on string and call func recursively
        if (root->character[i])
        {
            sprintf(tmp, "%s%c", prefix, tolower(i));
            display_trie(fp, root->character[i], tmp);
        }
    }
}

int is_leaf(Word* node) {
    if (node->isLeaf != 0) {
        return 0;
    } else
        return 1;
}

void trie_free (Word *root) {

    int i;

    for (i= 0; i < ALPHA_SIZE; ++i) {
        if (root->character[i]) {
            trie_free(root->character[i]);
        }
    }
    free (root);
}

void free_t (Trie* trie) {
    if (trie) {
        if (trie->root) {
            trie_free (trie->root);
        }
        free (trie);
    }
}

void str_clean_cz (char* src) {
    char *dst = src;

    while (*src) {
        if (!(*src > 0 && *src < 65)) {
            *dst++ = (unsigned char) tolower(*src);
        }

        src++;
    }
    *dst = '\0';
}

void str_clean_eng (char* src) {
    char *dst = src;

    while (*src) {
        if (isalpha(*src)) {
            *dst++ = (unsigned char) tolower(*src);
        }

        src++;
    }
    *dst = '\0';
}

char *MAX_STRING;
char *find_stem(Word* root, char *word, char prefix[], int msf_value, int lvl)
{
    int i;
    char *stem;

    char tmp[strlen(prefix) + 2];

    if (lvl == 0) {
        MAX_STRING = "0";
    }

    if (root == NULL) {
        perror("ERROR: cant display TRIE");
        return "0";
    }

    // If node is leaf node -> display
    if (!is_leaf(root))
    {
        stem = longest_common_substring(word, prefix);

        if (strlen(stem) > 3 &&  root->count > msf_value) {
            if (strlen(MAX_STRING) < strlen(stem)) {
                MAX_STRING = strdup(stem);
            }
        }
        free(stem);
    }

    for (i = 0; i < ALPHA_SIZE; i++)
    {
        // if we found NOT NULL child, write it on string and call func recursively
        if (root->character[i])
        {
            sprintf(tmp, "%s%c", prefix, tolower(i));
            find_stem(root->character[i], word, tmp, msf_value, lvl+1);
        }
    }

    return MAX_STRING;
}
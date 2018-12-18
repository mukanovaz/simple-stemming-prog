#include "trie.h"
#include "lcs.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <regex.h>

/*----------------------------------
 * Vytvari novy Trie a allokuje pamet
 *----------------------------------*/
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

/*----------------------------------
 * Vytvari novy Trie node a allokuje pamet
 *----------------------------------*/
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
    node->count = 0;

    for (i = 0; i < ALPHA_SIZE; i++)
        node->character[i] = NULL;

    return node;
}

/*----------------------------------
 * Vklada novy Trie node do Trie
 *----------------------------------*/
void insert(Trie* trie, char* str) {
    Word *curr;
    int index;

    curr = trie->root;

    if (count_characters(str) < 3) {
        return;
    }

    str_clean_cz(str);

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


    curr->count++;
    trie->count++;
    curr->isLeaf = trie->count; // leaf == (non zero)
}

/*----------------------------------
 * Hleda slovo v Trie
 *----------------------------------*/
Word* search(Trie* trie, char* str)
{
    int index;

    // return 0 if Trie is empty
    if (trie->root == NULL)
        return NULL;

    Word* curr = trie->root;
    while (*str)
    {
        index = (unsigned char) *str;
        // go to next node
        curr = curr->character[index];

        // if string is invalid (reached end of path in Trie)
        if (curr == NULL)
            return NULL;

        // move to next character
        str++;
    }

    return curr;
}

/*----------------------------------
 * Zapisuje cely Trie do souboru
 *----------------------------------*/
void display_trie(FILE *fp, Word *root, char prefix[]) {
    int i;
    char tmp[strlen(prefix) + 2];

    if (!root) {
        return;
    }

    // If node is leaf node -> display
    if (!is_leaf(root))
    {
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

/*----------------------------------
 * Zjisti zda uzel je posledni
 *----------------------------------*/
int is_leaf(Word* node) {
    if (node->isLeaf != 0) {
        return 0;
    } else
        return 1;
}

/*----------------------------------
 * Rekurzivni funkce pro uvolneni kazdeho slova
 *----------------------------------*/
void trie_free (Word *root) {
    int i;

    for (i= 0; i < ALPHA_SIZE; ++i) {
        if (root->character[i]) {
            trie_free(root->character[i]);
        }
    }
    free (root);
}

/*----------------------------------
 * Uvolneni Trie
 *----------------------------------*/
void free_t (Trie* trie) {
    if (trie) {
        if (trie->root) {
            trie_free (trie->root);
        }
        free (trie);
    }
}

/*----------------------------------
 * Nechava v retezci jenom pismena
 *----------------------------------*/
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

/*----------------------------------
 * Nechava v retezci jenom pismena
 *----------------------------------*/
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

/*----------------------------------
 * Rekurzivni funkce pro porovnani slova se slovnikem korenu
 *----------------------------------*/
void find_stem(Word* root, char *word, char prefix[], int msf_value, char *MAX_STR, size_t stemSize)
{
    int i;
    char mystem[100];
    size_t akt = 0;

    char tmp[strlen(prefix) + 2];

    if (root == NULL) {
        perror("ERROR: cant display TRIE");
        return;
    }

    // If node is leaf node -> display
    if (!is_leaf(root))
    {
        if(strstr(word,prefix)!= NULL) {
            akt = strlen(prefix);
            if(stemSize < akt){
                stemSize = akt;
                memcpy(mystem, prefix, stemSize);
                mystem[stemSize] = '\0';
                sprintf(MAX_STR, "%s", mystem);
            }

        }
    }

    for (i = 0; i < ALPHA_SIZE; i++)
    {
        // if we found NOT NULL child, write it on string and call func recursively
        if (root->character[i])
        {
            stemSize = strlen(MAX_STR);
            sprintf(tmp, "%s%c", prefix, tolower(i));
            find_stem(root->character[i], word, tmp, msf_value, MAX_STR, stemSize);
        }
    }
}

/*----------------------------------
 * Vrati pocet pismen v retezci
 *----------------------------------*/
int count_characters (const char *word) {
    int count = 0;
    int i = 0;

    while (word[i] != '\0') {
        count++;
        i++;
    }
    return count;
}

/*----------------------------------
 * Najde v retezci mejdelsi slovo
 *----------------------------------*/
char *find_longest_word (char *str) {

    int i, j = 0, k = 0;
    int len[15], min, max;

    char str1[15][25];
    char *result = (char*) malloc(1024);

    for (i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == 32)
        {
            str1[k][j] = '\0';
            k++;
            j = 0;
        } else {
            str1[k][j] = str[i];
            j++;
        }
    }

    str1[k][j] = '\0';

    for (i = 0; i <= k; i++)
    {
        len[i] = strlen(str1[i]);
    }

    max = len[0];
    min = len[0];

    for (i = 1; i <= k; i++)
    {
        if (max < len[i])
        {
            max = len[i];
        }
        if (min > len[i])
        {
            min = len[i];
        }
    }

    for (i = 0; i <= k; i++)
    {
        if (strlen(str1[i]) == max)
        {
            sprintf(result, "%s", str1[i]);
            return result;
        }
    }
}
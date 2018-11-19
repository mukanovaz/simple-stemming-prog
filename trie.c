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

void insert(Trie* trie, char* str)
{
    Word* curr;
    int index;

    curr = trie->root;

    if (strlen(str) < 3) {
        free(str);
        return;
    }

    while (*str)
    {
        index = (unsigned char) letter_to_int(*str);
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

Word* search(Trie* trie, char* str)
{
    int index;
    Word* curr;
    // return 0 if Trie is empty
    if (trie == NULL)
        return NULL;

    curr = trie->root;

    // return 0 if Root is empty
    if (curr == NULL) {
        return NULL;
    }

    while (*str)
    {
        index = (unsigned int) letter_to_int(*str);
        // go to next node
        curr = curr->character[index];

        // if string is invalid (reached end of path in Trie)
        if (curr == NULL)
            return NULL;

        // move to next character
        str++;
    }

    // if current node is a leaf and we have reached the
    // end of the string, return 1
    if (curr->isLeaf != 0)
        return curr;
    else
        return NULL;
}

char *MAX_STRING;
char *find_stem(Word* root, char *word, char str[], int level, int msf_value)
{
    int i;
    char *stem;

    if (level == 0) {
        MAX_STRING = "0";
    }

    if (root == NULL) {
        perror("ERROR: cant display TRIE");
        return "0";
    }

    // If node is leaf node -> display
    if (!is_leaf(root))
    {
        str[level] = '\0';
        stem = longest_common_substring(word, str);
        if (strlen(stem) > 3 &&  root->count > msf_value) {
            if (strlen(MAX_STRING) < strlen(stem)) MAX_STRING = stem;
//            printf("%s -> %s %d\n", word, stem, root->count);
        }
    }

    for (i = 0; i < ALPHA_SIZE; i++)
    {
        // if we found NOT NULL child, write it on string and call func recursively
        if (root->character[i])
        {
            str[level] = i + 'a';
            find_stem(root->character[i], word, str, level + 1, msf_value);
        }
    }
    return MAX_STRING;
}

void display(FILE *fp, Word* root, char str[], int level)
{
    int i;

    if (root == NULL) {
        perror("ERROR: cant display TRIE");
        return;
    }

    // If node is leaf node -> display
    if (!is_leaf(root))
    {
        str[level] = '\0';
        fprintf(fp, "%s %d\n", str, root->count);
    }

    for (i = 0; i < ALPHA_SIZE; i++)
    {
        // if we found NOT NULL child, write it on string and call func recursively
        if (root->character[i])
        {
            str[level] = i + 'a';
            display(fp, root->character[i], str, level + 1);
        }
    }
}

int is_leaf(Word* node) {
    if (node->isLeaf != 0) {
        return 0;
    } else
        return 1;
}

void trie_free (Word* root) {

    // stop recurse
    if (root == NULL)
    {
        //perror("ERROR: cant free TRIE");
        return;
    }
    else
    {
        for (int i = 0; i < ALPHA_SIZE; i++)
        {
            trie_free(root->character[i]);
        }
        free(root);
    }
}

void free_t (Trie* trie) {
    trie_free(trie->root);
    free(trie);
}

void lower_string(char *s) {
    int c = 0;

    while (s[c] != '\0') {
        if (s[c] >= 'A' && s[c] <= 'Z') {
            s[c] = s[c] + 32;
        }
        c++;
    }
}

void str_clean (char* src) {
    char *dst = src;

    while (*src) {
        if (islower(*src)) {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
}

void cz_key_map(int cz_chars[15])
{
    //						'á',  'č',  'ď', 'é',  'ě', 'í',  'ň', 'ó',  'ř', 'š', 'ť', 'ú', 'ů', 'ý', 'ž'
    // 						-95, -115, -113, -87, -101, -83, -120, -77, -103, -95, -91, -70, -81, -67, -66
    // 				  	     97,   99,  100, 101,  101, 105,  110, 111,  114, 115, 116, 117, 117, 121, 122

    cz_chars[-95] = 97 ;  // 'á'
    cz_chars[-115] = 99 ; // 'č'
    cz_chars[-113] = 100; // 'ď'
    cz_chars[-87] = 101;  // 'é'
    cz_chars[-101] = 101; // 'ě'
    cz_chars[-83] = 105;  // 'í'
    cz_chars[-120] = 110; // 'ň'
    cz_chars[-77] = 111;  // 'ó'
    cz_chars[-103] = 114; // 'ř'
    cz_chars[-95] = 115;  // 'š'
    cz_chars[-91] = 116;  // 'ť'
    cz_chars[-70] = 117;  // 'ú'
    cz_chars[-81] = 117;  // 'ů'
    cz_chars[-67] = 121;  // 'ý'
    cz_chars[-66] = 122;  // 'ž'
}

int letter_to_int(char letter)
{
    if (letter >= 'A' && letter <= 'Z')
    {
        return letter - 'A' + 26;
    }
    else if (letter >= 'a' && letter <= 'z')
    {
        return letter - 'a';
    }
    return -1;
}
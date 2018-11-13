#include "trie.h"
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
    char *tmp;
    Word* curr;
    int cz_chars[15];

    cz_key_map(cz_chars); // Get cz char map
    curr = trie->root;

    // Check string
    tmp = (char *) malloc(sizeof(tmp));
    strncpy(tmp, str, sizeof(str));
    lower_string(tmp);
    str_clean(tmp);

    if (strlen(tmp) < 1) {
        free(tmp);
        return;
    }

    while (*tmp)
    {
        // create a new node if path doesn't exists
        if (curr->character[*tmp - 'a'] == NULL) {
            curr->character[*tmp - 'a'] = get_new_trie_node();
        }
        // go to next node
        curr = curr->character[*tmp - 'a'];
        // move to next character
        tmp++;
    }

    trie->count++;
    curr->count++;
    curr->isLeaf = trie->count; // leaf == (non zero)
}

Word* search(Trie* trie, char* str)
{
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
        // go to next node
        curr = curr->character[*str - 'a'];

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

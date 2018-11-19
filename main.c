#include "library.h"
#include "trie.h"
#include "LinkedList.h"
#include "lcs.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <regex.h>

// /home/janelle/Projects/kiv-pc/corpus-files/dasenka_cili_zivot_stenete.txt -msl=5
// "rehabilitating accessible improved ahoj" -msf=20

/* Global variables */
int min_delka_korene;
int min_pocet_vyskytu_korenu;
Trie *trie;
List *current, *head;

int main(int argc, char *argv[]) {

    char* regex;
    char* file_path_regrex;

    trie = trie_initialize();
    file_path_regrex = "(\\\\?([^\\/]*[\\/])*)([^\\/]+).txt$";

    /* Bez parametru -msl nebo -msf */
    if (argc == 2) {

        /* UCENI STEMMERU */
        if (match(argv[1], file_path_regrex) == 1) {
            // Nastaveni parametru -msl=3
            printf("%s \nDefault -msl=3\n", argv[1]);
            min_delka_korene = IMPLICITNI_MIN_DELKA_KORENE;
            printf("+====================================================================================+\n");
            printf("|================================= TRENOVANI KORENU =================================|\n");
            printf("|====================================================================================|\n");
            printf(" [*] Path to corpus-file: %s    \n", argv[1]);
            printf(" [*] Minimum Stem Length: %d    \n", min_delka_korene);
            printf("+====================================================================================+\n");
            create_dictionary(argv[1]);
            create_words_array(trie->root, "");
            create_stems_file(min_delka_korene);
            destroy(head);
            free_t(trie);
        } else {                                    /* URCENI KORENU */
            printf("%s \nDefault -msf=10\n", argv[1]);
            // Nastaveni parametru -msf=10
            min_pocet_vyskytu_korenu = IMPLICITNI_MIN_POCET_VYSKYTU_KORENE;
            printf("+====================================================================================+\n");
            printf("|================================= URCOVANI KORENU ==================================|\n");
            printf("|====================================================================================|\n");
            printf(" [*] Path to corpus-file: %s    \n", argv[1]);
            printf(" [*] Minimum Stem Frequency: %d \n", min_pocet_vyskytu_korenu);
            printf("+====================================================================================+\n");
            create_stems_dictionary(STEMS);
            find_stems(argv[1], min_pocet_vyskytu_korenu);
            free_t(trie);
        }
    } else if (argc == 3) {
        if (match(argv[2], "-(msl)=[1-9]+") == 1) {
            printf("%s \nDefault %s\n", argv[1], argv[2]);
            min_delka_korene = get_parameter_value(argv[2]);
            printf("+====================================================================================+\n");
            printf("|================================= TRENOVANI KORENU =================================|\n");
            printf("|====================================================================================|\n");
            printf(" [*] Path to corpus-file: %s    \n", argv[1]);
            printf(" [*] Minimum Stem Length: %d    \n", min_delka_korene);
            printf("+====================================================================================+\n");
            create_dictionary(argv[1]);
            create_words_array(trie->root, "");
            create_stems_file(min_delka_korene);
            free_t(trie);
            destroy(head);
        } else if (match(argv[2], "-(msf)=[1-9]+")) {
            printf("%s \nDefault -msf=10\n", argv[1]);
            // Nastaveni parametru -msf=10
            min_pocet_vyskytu_korenu = get_parameter_value(argv[2]);
            printf("+====================================================================================+\n");
            printf("|================================= URCOVANI KORENU ==================================|\n");
            printf("|====================================================================================|\n");
            printf(" [*] Path to corpus-file: %s    \n", argv[1]);
            printf(" [*] Minimum Stem Frequency: %d \n", min_pocet_vyskytu_korenu);
            printf("+====================================================================================+\n");
            create_stems_dictionary(STEMS);
            find_stems(argv[1], min_pocet_vyskytu_korenu);
            free_t(trie);
        } else {
            using();
            return EXIT_FAILURE;
        }
    } else {
        using();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

//*|================================= TRENOVANI KORENU =================================|*//

void create_dictionary(char *corpus_file_path) {
    char * line = NULL;
//    char *tmp;
    char * word;
    size_t len = 0;
    ssize_t read;
    FILE *fp;       // File to read
    FILE *fp_dictionary;      // File to write

    // Open file to read
    fp = fopen(corpus_file_path, "r");
    if (fp == NULL)
    {
        perror("ERROR: Invalid filename");
        using();
        exit(1);
    }

    // Open file to write
    fp_dictionary = fopen(DICTIONARY, "w");
    if (fp_dictionary == NULL)
    {
        perror("ERROR: Invalid filename");
        using();
        exit(1);
    }

    // read line by line
    while ((read = getline(&line, &len, fp)) != -1) {
        // take words
        word = strtok(line, " ");
        while (word != NULL)
        {
            if (strcmp(word, "\n") == 0) {
                word = strtok (NULL, " ");
                continue;
            }

            // Insert to TRIE
            insert(trie, word);
            word = strtok (NULL, " ");
        }
    }

    if (line)
        free(line);

    display_trie(fp_dictionary, trie->root, "");
    fclose(fp);
    fclose(fp_dictionary);
    printf("Tries nodes count: %d\n", trie->count);
}

void create_words_array(Word *root, char prefix[]) {
    int i;
    char tmp[strlen(prefix) + 2];

    if (!root) {
        return;
    }

    // If node is leaf node -> display
    if (!is_leaf(root))
    {
        List *node = malloc(sizeof(List));
        node->string = strdup(prefix);
        node->next = NULL;

        if(head == NULL){
            current = head = node;
        } else {
            current = current->next = node;
        }
    }

    for (i = 0; i < ALPHA_SIZE; i++)
    {
        // if we found NOT NULL child, write it on string and call func recursively
        if (root->character[i])
        {
            sprintf(tmp, "%s%c", prefix, tolower(i));
            create_words_array(root->character[i], tmp);
        }
    }
}

void create_stems_file(int min_delka_korene) {
    FILE *fp;
    Trie *stems;
    int level = 0;
    char str[20];

    fp = fopen(STEMS, "w");
    if (fp == NULL)
    {
        perror("Error opening file!");
        exit(1);
    }

    stems = trie_initialize();

    compare_strings(fp, stems, head, min_delka_korene);

    display_trie(fp, stems->root, "");
    fclose(fp);
    free_t(stems);
}

void compare_strings(FILE *fp, Trie* trie, List *head, int min_delka_korene) {
    List * current = head;
    List * next;
    char *str;

    while (current != NULL) {
        next = head->next;
        while (next != NULL) {
            if (strcmp(current->string, next->string) != 0) {
                str = longest_common_substring(current->string, next->string);
                if (strlen(str) >= min_delka_korene){
                    insert(trie, str);
                    //fprintf(fp, "[%s] [%s] = [%s]\n", current->string, next->string, longest_common_substring(current->string, next->string));
                }
                free(str);
            }
            next = next->next;
        }
        if (current->next == NULL) break;
        current = current->next;
    }
}

/* |================================= URCOVANI KORENU ==================================| */

void find_stems(char *word_sequence, int msf_value) {
    char* words;
    int level = 0;
    char str[20];

    words = strtok(word_sequence, " ");
    while (words != NULL)
    {
        printf("%s -> %s\n", words, find_stem(trie->root, words, str, level, msf_value));
        words = strtok (NULL, " ,.-");
    }
    free_t(trie);
}

void create_stems_dictionary(const char *filename) {
    char *line = NULL;
    char *word[3];
    size_t len = 0;
    ssize_t read;
    FILE *fp;            // File to read
    FILE *fp2;            // File to read

    // Open file
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("Error opening file!");
        exit(1);
    }

    // Open file
    fp2 = fopen("/home/janelle/Projects/kiv-pc/output/out.txt", "w");
    if (fp2 == NULL)
    {
        perror("Error opening file!");
        exit(1);
    }

    // read line by line
    while ((read = getline(&line, &len, fp)) != EOF) {
        // take words
        word[0] = strtok(line, " ");
        word[1] = strtok(NULL, " ");

        for (int i = 0; i < atoi(word[1]); ++i) {
            insert(trie, word[0]);
        }
    }

    if (line)
        free(line);

    display_trie(fp2, trie->root, "");
    fclose(fp);
}

/* |======================================= MAIN ========================================| */
int get_parameter_value(char *string) {
    char delim[] = "=";
    char *ptr = strtok(string, delim);
    while (ptr != NULL)
    {
        if (match(ptr, "[0-9]+") == 1) {
            return atoi(ptr);
        }
        ptr = strtok(NULL, delim);
    }
    return 0;
}

int match(char *string, char *pattern)
{
    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED|REG_NOSUB) != 0) return 0;
    int status = regexec(&re, string, 0, NULL, 0);
    regfree(&re);
    if (status != 0) return 0;
    return 1;
}

int numbers_only(const char *s)
{
    while (*s) {
        if (isdigit(*s++) == 0) return 0;
    }
    return 1;
}

void using() {
    printf("+====================================================================================+\n");
    printf("| sistem.exe corpus-file | \"word-sequence\" [-msl=celé císlo] [-msf=celé císlo]       |\n");
    printf("| ---------------------------------------------------------------------------------- |\n");
    printf("| [*] sistem.exe                 nazev programu                                      |\n");
    printf("| ---------------------------------------------------------------------------------- |\n");
    printf("| [*] corpus-file                cesta k souboru (default pripona .txt)              |\n");
    printf("| [*] -msl <cele cislo>          minimalni delka korene slova                        |\n");
    printf("| ---------------------------------------------------------------------------------- |\n");
    printf("| [*] [\"]word-sequence[\"]        slovo nebo sekvenci slov                            |\n");
    printf("| [*] -msf <cele cislo>          minimalni pocet vyskytu prislusneho korene          |\n");
    printf("+====================================================================================+\n");
}


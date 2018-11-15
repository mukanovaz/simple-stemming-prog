#include "library.h"
#include "trie.h"
#include "lcs.h"
#include "LinkedList.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <regex.h>

/* Global variables */
int min_delka_korene;
int min_pocet_vyskytu_korenu;
Trie *trie;
List *current, *head;

int numbers_only(const char *s)
{
    while (*s) {
        if (isdigit(*s++) == 0) return 0;
    }
    return 1;
}

int main(int argc, char *argv[]) {

    char* regex;
    char* file_path_regrex;

    trie = trie_initialize();
    //compare_strings(head);

    file_path_regrex = "(\\\\?([^\\/]*[\\/])*)([^\\/]+)$";

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
            //create_words_array();
            //create_stems_dictionary(min_delka_korene);
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
            find_stems(argv[1], min_pocet_vyskytu_korenu);
        }
    } else if (argc == 3) {
                                                    /* UCENI STEMMERU */
        if (match(argv[1], file_path_regrex) == 1) {
            printf("%s \n%s\n", argv[1], argv[2]);

            /* Kontrolovani parametru */
            regex = "-(msl)=[1-9]+";
            if (match(argv[2], regex) == 1) {       /* Spravny parametr -msl */
                min_delka_korene = get_parameter_value(argv[2]);
            }
            else {                                  /* Spatny parametr -msl */
                using();
                destroy(head);
                free_t(trie);
                return EXIT_FAILURE;
            }

            printf("+====================================================================================+\n");
            printf("|================================= TRENOVANI KORENU =================================|\n");
            printf("|====================================================================================|\n");
            printf(" [*] Path to corpus-file: %s    \n", argv[1]);
            printf(" [*] Minimum Stem Length: %d    \n", min_delka_korene);
            printf("+====================================================================================+\n");
            create_dictionary(argv[1]);
        } else {                                    /* URCENI KORENU */
            printf("\"%s\" \n%s\n", argv[1], argv[2]);

            /* Kontrolovani parametru */
            regex = "-(msf)=[1-9]+";
            if (match(argv[2], regex) == 1) {       /* Spravny parametr -msf */
                min_pocet_vyskytu_korenu = get_parameter_value(argv[2]);
            }
            else {                                  /* Spatny parametr -msf */
                using();
                destroy(head);
                free_t(trie);
                return EXIT_FAILURE;
            }

            printf("+====================================================================================+\n");
            printf("|================================= URCOVANI KORENU ==================================|\n");
            printf("|====================================================================================|\n");
            printf(" [*] Path to corpus-file: %s    \n", argv[1]);
            printf(" [*] Minimum Stem Frequency: %d \n", min_pocet_vyskytu_korenu);
            printf("+====================================================================================+\n");
            find_stems(argv[1], min_pocet_vyskytu_korenu);
        }
    } else {
        using();
        destroy(head);
        free_t(trie);
        return EXIT_FAILURE;
    }
    destroy(head);
    free_t(trie);
    return EXIT_SUCCESS;
}

void create_test_trie () {
    FILE *fp;
    fp = fopen("/home/janelle/CLionProjects/sp_pc_2018/output/output.txt", "w");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    int level = 0;
    char str[20];

    insert(trie, "hello");
    insert(trie, "HELLO");
    insert(trie, "hell");
    insert(trie, "heLlO");
    insert(trie, "hello");
    insert(trie, "o");
    insert(trie, "lo");
    insert(trie, "alo");
    insert(trie, "hello");

    if (search(trie, "hello") != NULL) {
        Word* tmp = search(trie, "hello");
        printf("Node hello Count: %d \n", tmp->count);
    }

    printf("Tries nodes count: %d\n", trie->count);

    printf("\n");

    // display(fp, trie->root, str, level);
    free_t(trie);
    fclose(fp);
}

void create_dictionary(char *corpus_file_path) {
    char * line = NULL;
    int level = 0;
    char str[20];
    char * word;
    size_t len = 0;
    ssize_t read;
    FILE *fp;       // File to read
    FILE *fp2;      // File to write

    // Open file
    fp2 = fopen("/home/janelle/CLionProjects/sp_pc_2018/output/dictionary.txt", "w");
    if (fp2 == NULL)
    {
        perror("Error opening file!");
        exit(1);
    }

    // Open file
    fp = fopen(corpus_file_path, "r");
    if (fp == NULL)
    {
        perror("Error opening file!");
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
            insert(trie, word);
            word = strtok (NULL, " ");
        }
    }

    if (line)
        free(line);

    display(fp2, trie->root, str, level);
    fclose(fp);
    fclose(fp2);
    printf("Tries nodes count: %d\n", trie->count);
}

void find_stems(char *word_sequence, int msf_value) {
    char* words;
    words = strtok(word_sequence, " ");
    while (words != NULL)
    {
        printf ("%s\n",words);
        words = strtok (NULL, " ,.-");
    }
}

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

void create_stems_dictionary(int min_delka_korene) {

    Trie *stems;

    stems = trie_initialize();
    compare_strings(head, min_delka_korene);
    free_t(stems);
}

void create_words_array() {
    char *line = NULL;
    char *word;
    size_t len = 0;
    ssize_t read;
    FILE *fp;       // File to read

    // Open file
    fp = fopen("/home/janelle/CLionProjects/sp_pc_2018/output/dictionary.txt", "r");
    if (fp == NULL)
    {
        perror("Error opening file!");
        exit(1);
    }

    // read line by line
    while ((read = getline(&line, &len, fp)) != EOF) {
        // take words
        word = strtok(line, " ");
        while (word != NULL)
        {
            str_clean(word);

            if (strcmp(word, "") == 0){
                word = strtok (NULL, " ");
                continue;
            }
            if (strcmp(word, "\n") == 0) {
                word = strtok (NULL, " ");
                continue;
            }
            if (numbers_only(word)) {
                word = strtok (NULL, " ");
                continue;
            }

            List *node = malloc(sizeof(List));
            node->string = strdup(line);
            node->next =NULL;

            if(head == NULL){
                current = head = node;
            } else {
                current = current->next = node;
            }
            word = strtok (NULL, " ");
        }
    }

    if (line)
        free(line);

    fclose(fp);
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


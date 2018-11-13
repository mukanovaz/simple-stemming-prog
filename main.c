#include "library.h"
#include "trie.h"
#include "lcs.h"
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

int main(int argc, char *argv[]) {

    trie = trie_initialize();

    char* regex;
    char* file_path_regrex;

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
            create_stems_dictionary(min_delka_korene);
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
        free_t(trie);
        return EXIT_FAILURE;
    }
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

    display(fp, trie->root, str, level);
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
            //if (word == '\n');
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
    char array[trie->count];
    int i, j;
    FILE *fp;      // File to write
    char *str1 = (char *) malloc(sizeof(str1));
    char *str2 = (char *) malloc(sizeof(str2));

    // Open file
    fp = fopen("/home/janelle/CLionProjects/sp_pc_2018/output/stems.txt", "w");
    if (fp == NULL)
    {
        perror("Error opening file!");
        exit(1);
    }

    stems = trie_initialize();
    create_words_array(array);

    for (i = 0; i < sizeof(array) / sizeof(char); i++) {
        for (j = 0; j < sizeof(array) / sizeof(char); j++) {
            if (i == j) continue;
            strcpy(str1, &array[i]);
            strcpy(str2, &array[j]);
            find_lcs(str1, str2, sizeof(str1), sizeof(str2), min_delka_korene, stems);
        }
    }

    int level = 0;
    char str[20];
    display(fp, stems->root, str, level);

    fclose(fp);
    free_t(stems);
    free(str1);
    free(str2);
}

void create_words_array(char *array) {
    int i = 0;
    char * line = NULL;
    char * word;
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
    while ((read = getline(&line, &len, fp)) != -1) {
        // take words
        word = strtok(line, " ");
        while (word != NULL)
        {
            if (strcmp(word, "\n") == 0) {
                word = strtok (NULL, " ");
                continue;
            }

            if (isdigit(word) == 1) {
                continue;
            }
            strcpy(array[i], word);
            i++;
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


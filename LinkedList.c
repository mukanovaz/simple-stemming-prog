#include <stdlib.h>
#include <stdio.h>
#include "LinkedList.h"
#include "lcs.h"

void compare_strings(List *head, int min_delka_slova) {
    List * current = head;

    while (current != NULL) {
        while (current->next != NULL) {
            find_lcs(current->string, current->next->string, sizeof(current->string), sizeof(current->next->string), min_delka_slova);
            current = current->next;
            current->next = current->next->next;
        }
    }
}

void destroy(List * list) {
    List * current = list;
    List * next = current->next;
    while(current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
    free(list);
}
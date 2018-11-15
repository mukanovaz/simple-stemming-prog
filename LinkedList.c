#include <stdlib.h>
#include <stdio.h>
#include "LinkedList.h"
#include "lcs.h"
#include "trie.h"

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
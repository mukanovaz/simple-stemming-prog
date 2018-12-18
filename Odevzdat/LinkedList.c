#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "LinkedList.h"
#include "lcs.h"
#include "trie.h"

Node * create_node(char *data){
    Node * newNode = malloc(sizeof(Node));
    if (!newNode) {
        return NULL;
    }
    newNode->data = strdup(data);
    newNode->next = NULL;
    return newNode;
}

List * make_list(){
    List * list = malloc(sizeof(List));
    if (!list) {
        return NULL;
    }
    list->head = NULL;
    return list;
}

void insert_to_list(char *data, List *list){
    Node * current = NULL;
    if(list->head == NULL){
        list->head = create_node(data);
    }
    else {
        current = list->head;
        while (current->next!=NULL){
            current = current->next;
        }
        current->next = create_node(data);
    }
}

void destroy(List * list){
    Node * current = list->head;
    Node * next = current;
    while(current != NULL){
        next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(list);
}
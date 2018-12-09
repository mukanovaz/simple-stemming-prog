#ifndef SP_PC_2018_LINKEDLIST_H
#define SP_PC_2018_LINKEDLIST_H

typedef struct node Node;
typedef struct list List;

struct node {
    char *data;
    struct node * next;
};

struct list {
    Node * head;
};

List * make_list();
Node * create_node(char *data);
void insert_to_list(char *data, List *list);
void destroy(List * list);

#endif //SP_PC_2018_LINKEDLIST_H

#ifndef SP_PC_2018_LINKEDLIST_H
#define SP_PC_2018_LINKEDLIST_H

struct LIST;
typedef struct LIST List;

struct LIST {
    char *string;
    struct LIST *next;
};

void compare_strings(List *head, int min_delka_slova);
void destroy(List * list);

#endif //SP_PC_2018_LINKEDLIST_H

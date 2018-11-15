#ifndef SP_PC_2018_LINKEDLIST_H
#define SP_PC_2018_LINKEDLIST_H

#include <bits/types/FILE.h>


struct LIST;
typedef struct LIST List;

struct LIST {
    char *string;
    struct LIST *next;
};

void destroy(List * list);

#endif //SP_PC_2018_LINKEDLIST_H

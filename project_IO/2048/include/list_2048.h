#ifndef _LIST_2048_H_
#define _LIST_2048_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int element_t;

typedef struct list_2048{
    int x_len[4];
    int y_len[4];
    element_t item[4][4];
}list_2048, *list_2048_t;

list_2048_t list_2048_init(list_2048_t head);
list_2048_t list_2048_create(list_2048_t *new_list);
list_2048_t list_2048_set(list_2048_t head, int x, int y, element_t item);
int list_2048_travel(list_2048_t head);
list_2048_t list_2048_destroy(list_2048_t *head);

int list_2048_move_left(list_2048_t head);



#endif
#ifndef _LIST_2048_H_
#define _LIST_2048_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "list.h"

typedef int elen_2048_t;

typedef struct list_2048{
    elen_2048_t item[4][4];        //4x4的格子
    int len;                                           //格子被占用的数量
    int score;                                      //分数
}list_2048, *list_2048_t;

list_2048_t list_2048_init(list_2048_t head);
list_2048_t list_2048_create(list_2048_t *new_list);
int list_2048_set(list_2048_t head, int x, int y, elen_2048_t item);
int list_2048_travel(list_2048_t head);
list_2048_t list_2048_destroy(list_2048_t *head);
int list_2048_randcreate(list_2048_t head);

int list_2048_move_left(list_2048_t head);
int list_2048_move_right(list_2048_t head);
int list_2048_move_above(list_2048_t head);
int list_2048_move_below(list_2048_t head);
int list_2048_full(list_2048_t head);

int list_2048_save(list_2048_t game);
int list_2048_load(list_2048_t game);

int list_2048_record(node_t game_log, list_2048_t game);

#endif
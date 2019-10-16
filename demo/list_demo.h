#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    void *item;
    struct node *next;
    struct node *prev;
} node, *node_t;

typedef struct hahaha
{
    int s;
    int m;
} hahaha, *hahaha_t;

#endif
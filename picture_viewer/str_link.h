#ifndef _STR_LINK_H_
#define _STR_LINK_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char * element_t;

typedef struct node{
    element_t item;
    struct node *next;
    struct node *prev;
}node, *node_t;

node_t link_create(node_t *head);
node_t link_init(node_t head);
node_t link_destroy(node_t *head);
int link_travel(node_t head);
node_t node_get(node_t head, int position);
node_t node_insert(node_t head, element_t item, int position);
node_t node_delete(node_t head, int position);

#endif
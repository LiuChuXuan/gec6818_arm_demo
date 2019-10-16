#ifndef _LIST_H_
#define _LIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    void *item;
    int size;
    struct node *next;
    struct node *prev;
} node, *node_t;

int list_length(node_t head);
node_t list_init(node_t head);
node_t list_create(node_t *head);
node_t list_destroy(node_t *head);
node_t list_insert_node(node_t head, void *item, int size, int position);
node_t list_get_node(node_t head, int position);
node_t list_delete_node(node_t head, int position);

#endif
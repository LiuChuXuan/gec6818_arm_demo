#include "list.h"

//返回链表的长度
int list_length(node_t head)
{
    if(head == NULL)
    {
        perror("list length == NULL");
        return -1;
    }

    int count = 0;
    node_t temp = head->next;
    while(temp != head)
    {
        count++;
        temp = temp->next;
    }
    return count;
}

//初始化链表
node_t list_init(node_t head)
{
    if(head == NULL)
    {
        perror("list init == NULL\n");
        return head;
    }
    node_t temp = head->next;
    while(temp != head)
    {
        head->next = temp->next;
        temp->next->prev = head;
        free(temp->item);
        free(temp);
        temp = head->next;
    }

    head->item = NULL;
    head->size = 0;
    head->next = head;
    head->prev = head;
    return head;
}

//创建(堆空间)链表，并且将他初始化，返回头节点指针
node_t list_create(node_t *head)
{
    *head = (node_t)malloc(sizeof(node));
    if(*head == NULL)
    {
        perror("list_create == NULL\n");
        return *head;
    }

    (*head)->item = NULL;
    (*head)->size = 0;
    (*head)->next = *head;
    (*head)->prev = *head;
    return *head;
}

node_t list_destroy(node_t *head)
{
    if(*head == NULL)
    {
        perror("list destroy == NULL");
        return *head;
    }
    list_init(*head);
    free(*head);
    *head = NULL;
    return *head;
}

//insert by position,插入到第n个，n = position.
node_t list_insert_node(node_t head, void *item, int size, int position)
{
    if(position == 0)
    {
        perror("node_insert不能插入到头结点，第三个参数不能为0");
        return head;
    }

    if(head == NULL)
    {
        perror("list insert == NULL");
        return head;
    }

    int count = 0;
    node_t temp = head;
    int length = list_length(head);

    if((position < 0 ? -position-1 : position-1) > length)
    {
        perror("所给位置超过链表长度");
        return head;
    }

    node_t new_node = (node_t)malloc(sizeof(node));
    if(new_node == NULL)
    {
        perror("malloc new node failed");
        return head;
    }
    memset(new_node, 0, sizeof(node));
    new_node->item = item;
    new_node->size = size;

    if(position > 0)
    {
        for(count = 0; count < position-1; count++)
        {
            temp = temp->next;
        }
        new_node->next = temp->next;
        new_node->prev = temp;
        temp->next->prev = new_node;
        temp->next = new_node;
    }
    else if(position < 0)
    {
        for(count = 0; count > position+1; count--)
        {
            temp = temp->prev;
        }
        new_node->next = temp;
        new_node->prev = temp->prev;
        temp->prev->next = new_node;
        temp->prev = new_node;
    }
    return new_node;
}

//get node by position
node_t list_get_node(node_t head, int position)
{
    if(head == NULL)
    {
        perror("list_get_node == NULL");
        return head;
    }

    int count = 0;
    node_t temp = head;
    int length = list_length(head);

    if((position < 0 ? -position : position) > length)
    {
        perror("所给位置超过链表长度");
        return NULL;
    }

    if(position > 0)
    {
        for(count = 0; count < position; count++)
        {
            temp = temp->next;
        }
    }
    else if(position < 0)
    {
        for(count = 0; count > position; count--)
        {
            temp = temp->prev;
        }
    }
    return temp;
}

//删除第n个节点，n = position.
node_t list_delete_node(node_t head,  int position)
{
    if(position == 0)
    {
        perror("node_delete不能删除头结点，即第二个参数不能为0");
        return head;
    }
    
    node_t temp = list_get_node(head, position);

    temp->next->prev = temp->prev;
    temp->prev->next = temp->next;
    free(temp->item);
    free(temp);

    return head;
}

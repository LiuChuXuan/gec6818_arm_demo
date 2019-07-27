#include "str_link.h"

node_t link_create(node_t *head)
{
    *head = (node_t)malloc(sizeof(node));
    if(*head != NULL)
    {
        memset(*head, 0, sizeof(node));
        (*head)->item = 0;
        (*head)->next = *head;
        (*head)->prev = *head;
    }
    else
    {
        perror("link_create: head == NULL");
    }
    return *head;
}

node_t link_init(node_t head)
{
    if(head != NULL)
    { 
        node_t temp = head->next;
        while(temp != head)
        {
            head->next = temp->next;
            temp->next->prev = head;
            free(temp->item);
            free(temp);
            temp = head->next;
        }
    }
    else
    {
        perror("link_init: head == NULL");
    }
    return head;
}

node_t link_destroy(node_t *head)
{
    if(*head != NULL)
    {
        link_init(*head);
        free(*head);
        *head = NULL;
    }
    else
    {
        perror("link_destroy: head == NULL");
    }
    return *head;
}

int link_travel(node_t head)
{
    int count = 0;
    if(head != NULL)
    {
        node_t temp = head->next;
        while(temp != head)
        {
            count++;
            printf("%s\n", temp->item);
            temp = temp->next;
        }
    }
    else
    {
        perror("head == NULL");
        return -1;
    }
    return count;
}

int link_length(node_t head)
{
    int count = 0;
    if(head != NULL)
    {
        node_t temp = head->next;
        while(temp != head)
        {
            count++;
            temp = temp->next;
        }
    }
    else
    {
        perror("head == NULL");
        return -1;
    }
    return count;
}

//insert by position,插入到第n个，n = position.
node_t node_insert(node_t head, element_t item, int position)
{
    if(position == 0)
    {
        perror("node_insert不能插入到头结点，第三个参数不能为0");
        return head;
    }
    if(head != NULL)
    {
        int count = 0;
        node_t temp = head;
        int length = link_length(head);

        if((position < 0 ? -position-1 : position-1) > length)
        {
            perror("所给位置超过链表长度");
            return head;
        }

        node_t new_node = (node_t)malloc(sizeof(node));
        if(new_node == NULL)
        {
            perror("malloc new node failed");
            exit(-1);
        }
        memset(new_node, 0, sizeof(node));
        new_node->item = item;

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
    }
    else
    {
        perror("insert:head == null");
    }
    return head;
}

//get node by position
node_t node_get(node_t head, int position)
{
    if(head != NULL)
    {
        int count = 0;
        node_t temp = head;
        int length = link_length(head);

        if((position < 0 ? -position : position) > length)
        {
            perror("所给位置超过链表长度");
            return head;
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
    else
    {
        perror("node_get : head == NULL");
    }
    return head;
}

//删除第n个节点，n = position.
node_t node_delete(node_t head,  int position)
{
    if(position == 0)
    {
        perror("node_delete不能删除头结点，即第二个参数不能为0");
        return head;
    }
    
    node_t temp = node_get(head, position);

    temp->next->prev = temp->prev;
    temp->prev->next = temp->next;
    free(temp->item);
    free(temp);

    return head;
}
#include "list.h"

int main(void)
{
    node_t ret = NULL;
    node_t new_a = NULL;
    list_create(&new_a);

    int *data = (int *)malloc(sizeof(int));
    if (data == NULL)
    {
        perror("data == NULL");
        return -1;
    }

    *data = 2333;
    list_insert_node(new_a, data, sizeof(int), 1);

    printf("item = %d\n", *((int *)(new_a->next->item)));
    ret = list_destroy(&new_a);
    return 0;
}
#include "list.h"

int main(void)
{
    node_t aaaa_node = (node_t)malloc(sizeof(node));
    if (aaaa_node == NULL)
    {
        perror("aaaa_node == NULL");
        return -1;
    }
    memset(aaaa_node, 0, sizeof(node));

    // int -----------------------------------------
    int *data = (int *)malloc(sizeof(int));
    if (data == NULL)
    {
        perror("data == NULL");
        return -1;
    }

    *data = 2333;
    printf("aaa : %d\n", *data);

    aaaa_node->item = data;
    printf("bbbb = %d", *((int *)(aaaa_node->item)));

    free(aaaa_node->item);
    // struct -----------------------------------------
    aaaa_node->item = (hahaha_t)malloc(sizeof(hahaha));
    if (aaaa_node->item == NULL)
    {
        free(aaaa_node);
        perror("hahaha");
        return -1;
    }
    ((hahaha_t)(aaaa_node->item))->s = 10;
    printf("%d\n", ((hahaha_t)(aaaa_node->item))->s);

    free(aaaa_node->item);
    free(aaaa_node);
    return 0;
}
#include "main.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        perror("argc != 2");
        return -1;
    }

    node_t aaaa = NULL;
    link_create(&aaaa);

    scan_picture(argv[1], aaaa);

    node_t temp = NULL;

    temp = node_get(aaaa,1);
    printf("%s\n",temp->item);
    temp = node_get(aaaa,2);
    printf("%s\n",temp->item);
    temp = node_get(aaaa,3);
    printf("%s\n",temp->item);

    node_delete(aaaa,2);
    temp = node_get(aaaa,2);
    printf("%s\n",temp->item);

    //link_travel(aaaa);

    link_destroy(&aaaa);

    return 0;
}
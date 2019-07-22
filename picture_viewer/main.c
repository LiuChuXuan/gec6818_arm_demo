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
    printf("1111111111111111\n");

    char * a = (char*)malloc(10);
    memset(a,0,10);
    a = strcat(a,"123");
    printf("%s\n",a);

    node_insert(aaaa, a, -1);


    link_travel(aaaa);
    printf("21111111111111111\n");

    link_destroy(&aaaa);

    //ls_dir(argv[1]);
    return 0;
}
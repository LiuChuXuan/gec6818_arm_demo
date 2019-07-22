#include "main.h"

int main(int argc, char *argv[])
{
///*
    if(argc != 2)
    {
        perror("argc != 2");
        return -1;
    }
    ls_dir(argv[1]);
//*/
/* 
    node_t aaaa = NULL;
    link_create(&aaaa);

    scan_picture(argv[1], aaaa);
    //scan_picture("/home/yoling/lab/picture", aaaa);

    link_travel(aaaa);

    link_destroy(&aaaa);
*/
    return 0;
}
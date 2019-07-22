#include "main.h"

int main(int argc, char *argv[])
{
    node_t aaaa = NULL;
    link_create(&aaaa);


    node_insert(aaaa, 1, -1);
    node_insert(aaaa, 2, -1);
    node_insert(aaaa, 3, -1);
    node_insert(aaaa, 4, -1);
    node_insert(aaaa, 5, -1);

    int elen_num = link_travel(aaaa);
    printf("elen_num = %d\n", elen_num);

    node_insert(aaaa, 10, 6);
    elen_num = link_travel(aaaa);
    printf("elen_num = %d\n", elen_num);
    node_delete(aaaa,  3);
    printf("after delete: ");
    link_travel(aaaa);

    link_init(aaaa);

    elen_num = link_travel(aaaa);
    printf("elen_num = %d\n", elen_num);



    link_destroy(&aaaa);

/*
    if(argc != 2)
    {
        printf("argv != 2");
        return -1;
    }
    
    DIR *dp = opendir(path);
    struct dirent *p = NULL;
    if(dp == NULL)
    {
        perror("opendir failed");
        return -1;
    }
 */
    //show_bmp(argv[1],0, 0);
    //ls_dir(argv[1]);

/*
    int ret = 0;
    while(1)
    {
        ret = left_right();
        if(ret = 0)
        {
            printf("左划");
        }
        else if(ret = 1)
        {
            printf("右滑");
        }
    }
 */
    return 0;
}
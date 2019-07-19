#include "open_dir.h"
/*
int show_dir_bmp(char *path)
{
    DIR *dp = opendir(path);
    if(dp == NULL)
    {
        perror("opendir failed");
        return -1;
    }

    struct dirent *p = NULL;

    while(1)
    {
        p = readdir(dp);
        if(p == NULL)
        {
            break;
        }
        else
        {
            show_bmp(p->d_name, 0, 0)
        }
    }
    closedir(dp);
    return 0;
}
*/
int ls_dir(char *path)
{
    DIR *dp = opendir(path);
    if(dp == NULL)
    {
        perror("opendir failed");
        return -1;
    }

    struct dirent *p = NULL;

    while(1)
    {
        p = readdir(dp);
        if(p == NULL)
        {
            break;
        }
        else
        {
            printf("\n==============\n");
            printf("d_ino: %lu\n", p->d_ino);
            printf("d_off: %ld\n", p->d_off);
            printf("d_reclen: %d\n", p->d_reclen);
            printf("d_type: %d\n", p->d_type);
            printf("d_name: %s\n", p->d_name);
        }
    }
    closedir(dp);
    return 0;
}
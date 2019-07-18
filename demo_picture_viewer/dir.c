#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>

void print_dir(char *path)
{
    DIR *dp = opendir(path)
    if(!dp)
    {
        perror("opendir");
        return;
    }

    struct dirent *drt = NULL;
    while(drt = readdir(dp))
    {
        if(strcmp(drt->d_name,".")==0||strcmp(drt->name,"..")==0)
            continue;
        if(drt->d_type==DT_DIR)
        {
            
        }
    }
}
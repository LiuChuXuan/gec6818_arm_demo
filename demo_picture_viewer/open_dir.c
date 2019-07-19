#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

// 将指定目录中的文件名列出来
int main(int argc, char **argv) // ./directory dir/
{
    // 1，打开目录，得到目录指针（此时，并没有进入目录）
    DIR *dp = opendir(argv[1]);
    if(dp == NULL)
    {
        perror("指定的目录无法打开");
        exit(0);
    }

    // chdir() 用来进入目录

    // 2，循环读取目录项
    struct dirent *ep;
    while(1)
    {
        ep = readdir(dp);

        if(ep == NULL)
        {
            break;
        }

        printf("%s\n", ep->d_name);
    }

    // 3，释放相关资源
    closedir(dp);

    return 0;
}
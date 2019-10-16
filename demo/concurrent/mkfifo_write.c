#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

int main(void)
{
    //检测有名管道文件是否存在
    if (access("./myfifo", F_OK))
    {
        //创建有名管道
        if (mkfifo("./myfifo", 0777))
        {
            perror("mkfifo failed");
            return -1;
        }
        else
        {
            printf("create fifo success\n");
        }
    }
    else
    {
        perror("myfifo exixt");
    }

    //打开有名管道文件
    perror("111111111111111");
    int fifo_fd = open("./myfifo", O_WRONLY);
    perror("222222222222222");
    if (fifo_fd == -1)
    {
        perror("open myfifp failed");
        return -1;
    }
    else if (fifo_fd > 0)
    {
        printf("open myfifo success\n");
    }

    char buf[20] = "aaaaaa";

    printf("write %s\n", buf);

    //写入数据
    write(fifo_fd, buf, 20);

    //关闭文件
    close(fifo_fd);
    return 0;
}
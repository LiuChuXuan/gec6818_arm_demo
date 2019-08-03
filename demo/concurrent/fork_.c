#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/stat.h>
#include <string.h>

void func(void);

int main(void)
{
    char buf[20];
    memset(buf, 0, sizeof(buf));

    //申请一个int型数组用于存放pipefd
    int pipefd[2];
    memset(pipefd, 0, sizeof(pipefd));

    //将数组地址传进去，函数会将pipefd信息写入到地址里
    int ret_pipe = pipe(pipefd);
    if(ret_pipe == -1)//成功返回0，失败返回-1
    {
        perror("create pipe failed");
        return -1;
    }

    atexit(func);

    printf("hello\n");
    pid_t ret = fork();
    if(ret == 0)
    {
        printf("ret == 0 pid : %d\n",getpid());
        printf("ret == 0 ppid : %d\n",getppid());
        printf("parent\n");
        while(1)
        {
            read(pipefd[0], buf, 12);//从读端pipefd[0]读取数据到buf里面
            printf("%s\n", buf);
        }
    }
    else if(ret > 0)
    {
        printf("ret > 0 pid : %d\n",getpid());
        printf("ret > 0 ppid : %d\n",getppid());
        printf("child\n");

        while(1)
        {
            strcpy(buf,"aaaaaaaaaa\n");
            write(pipefd[1], buf, strlen(buf));//将buf里面的数据写入到写端pipefd[1]
            //sleep(1);
        }
    }
    else if(ret == -1)
    {
        perror("error");
    }

    printf("world\n");

    return 0;
}

void func(void)
{
    int a = 10;
    printf("a = %d\n",++a);
}
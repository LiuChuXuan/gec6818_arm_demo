#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void sighander(int sig)
{
    printf("\nsig = %d\n",sig);
    kill(getpid(), 9);
    //kill(getpid(), 2);
}

//typedef void (*sighandler_t)(int);
//sighandler_t signal(int signum, sighandler_t handler);

int main(void)
{
    //signal(2, SIG_IGN);

    //捕获2信号，捕捉到了之后执行sighander函数
    signal(2, sighander);

    while(1)
    {
        printf("a\n");
        sleep(1);
    }

    return 0;
}
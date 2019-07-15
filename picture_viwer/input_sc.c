#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>

int main(void)
{
    struct input_event evt;
    int fd = open("/dev/input/event0",O_RDWR);
    if(fd==-1)
    {
        perror("open");
        return -1;
    }

    int x,y;
    while(read(fd,&evt,sizeof(evt))>0)
    {
        if(evt.type==EV_ABS)
        {
            if(evt.code==ABS_X)
            {
                x = evt.value;
            }
            else if(evt.code == ABS_Y)
            {
                y = evt.value;
            }
        }

        if(evt.type==EV_KEY&&evt.value==1)
            printf("按下的坐标为(%d,%d)\n",x,y);
    }
    close(fd);
    return 0;
}
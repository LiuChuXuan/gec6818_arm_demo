#include "input_sc.h"


int get_xy()
{

}

//判断左划还是右划，左划返回0，右划返回1
int left_right(void)
{
    struct input_event evt;/*  /dev/input/event0  */  
   
    //打开输入设备文件
    int fd = open("/dev/input/event0",O_RDWR);
    if(fd==-1)
    {
        perror("open");
        return -1;
    }

    printf("fd = %d", fd);

    int flag = 0;//0x00000000  最后四位从左到右 x1 x2 y1 y2
    int x1 = 0, x2 = 0;
    int y1 = 0, y2 = 0;
    while(read(fd,&evt,sizeof(evt))>0)
    {
        if(evt.type==EV_ABS)
        {
            if(evt.code==ABS_X)
            {
                if((flag & 0x00001000) == 0x00001000)
                {
                    x1 = evt.value;
                    flag |= 0x00001000;
                }
                x2 = evt.value;
            }
            else if(evt.code == ABS_Y)
            {
                if((flag & 0x00000010) == 0x00000010)
                {
                    y1 = evt.value;
                    flag |= 0x00000010;
                }
                y2 = evt.value;
            }
        }

        if(evt.type==EV_KEY&&evt.value==1)
        {
            printf("(%d,%d)",x2,y2);
        }
    }
    printf("松开时的坐标为(%d,%d)\n",x2,y2);
    printf("按下时的坐标为(%d,%d)\n",x1,y1);

    if(x2 > x1)
    {
        close(fd);
        return 1;
    }
    
    if(x1 > x2)
    {
        close(fd);
        return 0;
    }


    return -1;
}
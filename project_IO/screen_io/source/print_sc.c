#include "print_sc.h"

static inline int circle_plot(sc_info_t screen, \
            unsigned int colour, int x_center, \
                int y_center, int x, int y)
{
    if(screen == NULL)
    {
        perror("circle plot screen == NULL");
        return -1;
    }
    if((x_center + x < screen->xres) && 
       (x_center + x > 0)            &&
       (y_center + y < screen->yres) &&
       (y_center + y > 0))
    {
        *(screen->fb + (x_center + x) + (y_center + y) * screen->xres) = colour;
        *(screen->fb + (x_center - x) + (y_center + y) * screen->xres) = colour;
        *(screen->fb + (x_center + x) + (y_center - y) * screen->xres) = colour;
        *(screen->fb + (x_center - x) + (y_center - y) * screen->xres) = colour;
        *(screen->fb + (x_center + y) + (y_center + x) * screen->xres) = colour;
        *(screen->fb + (x_center - y) + (y_center + x) * screen->xres) = colour;
        *(screen->fb + (x_center + y) + (y_center - x) * screen->xres) = colour;
        *(screen->fb + (x_center - y) + (y_center - x) * screen->xres) = colour;
    }
    return 0;
}

//输入颜色(ARGB各1byte),圆心(x, y)，半径r
int draw_circle(sc_info_t screen, unsigned int colour, int x_center, int y_center, int r)
{
    if(screen == NULL)
    {
        perror("draw_circle screen == NULL");
        return -1;
    }
    //快速画圆法画圆
    int x = 0, y = r, d = -r / 2;
    circle_plot(screen, colour, x_center, y_center, x, y);
   
    if(r % 2 == 0)
    {
        while(x < y)
        {
            x++;
            if(d < 0)
            {
                d += x;
            }
            else
            {
                y--;
                d += x - y;
            }
            circle_plot(screen, colour, x_center, y_center, x, y);
        }
    }
    else
    {
        while(x < y)
        {
            x++;
            if(d < 0)
            d += x + 1;
            else
            {
                y--;
                d += x - y + 1;
            }
            circle_plot(screen, colour, x_center, y_center, x, y);
        }
    }

    return 0;
}


/* //油漆桶功能, 将一个封闭的区域填充成同一种颜色, 最后一个参数为误差忽略
int fill_colour(sc_info_t screen, int *buff, \
unsigned int colour, int x, int y,  int ignore)
{
    unsigned int temp = 0;
    if(screen == NULL)
    {
        perror("fill_colour screen == NULL");
    }
    if(buff == NULL)
    {
        perror("fill colour == NULL");
        return -1;
    }
    while(1)
    {
        temp = *(screen->fb + x + y * screen->xres);
        temp = temp - colour;
        temp = temp >= 0 ? temp : -temp;
        if(temp > ignore)
        {
            
        }
    }
} */


//输入颜色(ARGB各1byte), 通过手指滑动屏幕画画
int draw(sc_info_t screen, unsigned int colour)
{
    //打开输入事件文件
    int fd_event0 = open("/dev/input/event0",O_RDWR);
    if(fd_event0 == -1)
	{
        perror("open event0 failed");
        return -1;
    }

    //定义一个结构体用于接收从输入事件得到的信息
    struct input_event evt;
	memset(&evt, 0, sizeof(evt));

    //定义一个flag用于标记是否已经读取了x, y
    int flag = 0, x = 0, y = 0;
    //不断的获取屏幕输入事件
    while(read(fd_event0, &evt, sizeof(evt)) > 0)
    {
        if(evt.type == EV_ABS)
        {
            if(evt.code == ABS_X)
            {
                x = evt.value;
                flag |= 1 << 1;
            }
            else if(evt.code == ABS_Y)
            {
                y = evt.value;
                flag |= 1 << 0;
            }
        }
        if(flag == 0B00000011)
        {
            printf("(%d,%d)\n",x,y);
            *(screen->fb + x + y*screen->xres) = colour;
            flag = 0;
        }
    }

    //释放资源
    close(fd_event0);
    return 0;
}
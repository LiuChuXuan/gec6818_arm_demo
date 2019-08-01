#include "print_sc.h"

static inline int circle_plot(unsigned int colour, int x_center, \
                int y_center, int x, int y, int *memp, int xres, int yres)
{
    if(memp == NULL)
    {
        perror("circle plot memp == NULL");
        return -1;
    }
    if((x_center + x < xres) && 
        (x_center + x > 0)       &&
        (y_center + y < yres) &&
        (y_center + y > 0))
    {
        *(memp + (x_center + x) + (y_center + y) * xres) = colour;
        *(memp + (x_center - x) + (y_center + y) * xres) = colour;
        *(memp + (x_center + x) + (y_center - y) * xres) = colour;
        *(memp + (x_center - x) + (y_center - y) * xres) = colour;
        *(memp + (x_center + y) + (y_center + x) * xres) = colour;
        *(memp + (x_center - y) + (y_center + x) * xres) = colour;
        *(memp + (x_center + y) + (y_center - x) * xres) = colour;
        *(memp + (x_center - y) + (y_center - x) * xres) = colour;
    }
    return 0;
}

//输入颜色(ARGB各1byte),圆心(x, y)，半径r
int draw_circle(unsigned int colour, int x_center, int y_center, int r)
{
    //打开Frame Buffer文件
    int fd_fb0 = open("/dev/fb0",O_RDWR);
    if(fd_fb0 == -1)
    {
        perror("open");
        return -1;
    }
    //定义一个结构体用于获取屏幕硬件信息
    struct fb_var_screeninfo var;
    memset(&var, 0, sizeof(var));
    //获取屏幕硬件信息
    ioctl(fd_fb0,FBIOGET_VSCREENINFO,&var);
    //内存映射
    int *memp = (int *)mmap(NULL,var.xres*var.yres*var.bits_per_pixel/8, \
                                PROT_WRITE|PROT_READ,MAP_SHARED,fd_fb0,0);

    if(memp == MAP_FAILED)
    {
        perror("mmap");
        return -2;
    }

    //快速画圆法画圆
    int x = 0, y = r, d = -r / 2;
    circle_plot(colour, x_center, y_center, x, y, memp, var.xres, var.yres);
   
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
            circle_plot(colour, x_center, y_center, x, y, memp, var.xres, var.yres);
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
            circle_plot(colour, x_center, y_center, x, y, memp, var.xres, var.yres);
        }
    }

    munmap(memp,var.xres*var.yres*var.bits_per_pixel/8);
    close(fd_fb0);
    return 0;
}

//油漆桶功能, 将一个封闭的区域填充成同一种颜色, 最后一个参数为误差忽略
int fill_colour(unsigned int colour, int x, int y, int *buff, int ignore, int xres, int yres)
{
    if(buff == NULL)
    {
        perror("fill colour == NULL");
        return -1;
    }
    while(1)
    {

    }

}

//输入颜色(ARGB各1byte), 通过手指滑动屏幕画画
int draw(unsigned int colour)
{
    //打开输入事件文件
    int fd_event0 = open("/dev/input/event0",O_RDWR);
    if(fd_event0 == -1)
	{
        perror("open event0 failed");
        return -1;
    }
    //打开Frame Buffer文件
    int fd_fb0 = open("/dev/fb0",O_RDWR);
    if(fd_fb0 == -1)
    {
        perror("open");
        return -1;
    }
    //定义一个结构体用于获取屏幕硬件信息
    struct fb_var_screeninfo var;
    memset(&var, 0, sizeof(var));
    //获取屏幕信息
    ioctl(fd_fb0,FBIOGET_VSCREENINFO,&var);
    //内存映射
    int *memp = (int *)mmap(NULL,var.xres*var.yres*var.bits_per_pixel/8, 
                                PROT_WRITE|PROT_READ,MAP_SHARED,fd_fb0,0);

    if(memp == MAP_FAILED)
    {
        perror("mmap");
        return -2;
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
            *(memp + x + y*var.xres) = colour;
            flag = 0;
        }
    }

    //释放资源
    munmap(memp,var.xres*var.yres*var.bits_per_pixel/8);
    close(fd_fb0);
    close(fd_event0);
    return 0;
}
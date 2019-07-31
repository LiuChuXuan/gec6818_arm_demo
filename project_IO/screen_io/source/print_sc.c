#include "print_sc.h"

//输入颜色(ARGB各1byte)
//在指定位置(x, y)画一个圆
//第二个参数是画笔大小，决定了圆的粗细，= -1时为填充
int draw_circle(unsigned int colour, int pixel, int x, int y, int r)
{
    //打开Frame Buffer文件
    int fd_fb0 = open("/dev/fb0",O_RDWR);
    if(fd_fb0 == -1)
    {
        perror("open");
        return -1;
    }
    //定义一个结构体用于获取屏幕信息
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






    munmap(memp,var.xres*var.yres*var.bits_per_pixel/8);
    close(fd_fb0);
    return 0;
}

//输入颜色(ARGB各1byte)，画笔大小
//通过手指滑动屏幕画画
int draw(unsigned int colour, int pixel)
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
    //定义一个结构体用于获取屏幕信息
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
    //不断的获取屏幕输入
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
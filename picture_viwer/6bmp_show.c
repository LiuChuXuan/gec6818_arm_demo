#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "bit_bmp.h"

int main(int argc, char *argv[])
{
    int ret = 0;
    struct bitmap_header bmp_header;
    struct bitmap_info bmp_info;
    memset(&bmp_header, 0, 14);
    memset(&bmp_info, 0, 40);

    if(argc != 2)
    {
        printf("argc != 2");
        return -1;
    }

    int fd_bmp = open(argv[1],O_RDONLY);
    if(fd_bmp == -1)
    {
        perror("open failed");
        return -1;
    }
    
    ret = read(fd_bmp, &bmp_header, 14);
    if(ret != 14)
    {
        perror("read error 14");
    }
    //printf("%d\n",bmp_header.type);
    //如果不是bmp格式则退出程序。
    if(bmp_header.type != 19778)
    {
        printf("It's not bmp file");
        return -1;
    }

    ret = read(fd_bmp, &bmp_info, 40);
    if(ret != 40)
    {
        perror("read error 40");
    }

    int fd_lcd = open("/dev/fb0",O_RDWR);
    if(fd_lcd == -1)
    {
        perror("open");
        return -1;
    }

    //打开lcd设备
    struct fb_var_screeninfo var;
    ioctl(fd_lcd,FBIOGET_VSCREENINFO,&var);

    printf("lcd_x = %d, lcd_y = %d, pixel = %d\n",var.xres,var.yres,var.bits_per_pixel);

    int *memp = (int *)mmap(NULL,var.xres*var.yres*var.bits_per_pixel/8, 
                                PROT_WRITE|PROT_READ,MAP_SHARED,fd_lcd,0);

    if(memp == MAP_FAILED)
    {
        perror("mmap");
        return -2;
    }

    printf("bmp_x = %d, bmp_y =  %d\n", bmp_info.width, bmp_info.height);
    unsigned int lcd_buf[ bmp_info.width * bmp_info.height];
    memset(lcd_buf, 0, sizeof(lcd_buf));

    unsigned char bmp_buf[3];
    memset(bmp_buf, 0, sizeof(bmp_buf));

    //将图片读取到bmp_buf再整合到lcd_buf中
    int h_count = 0, w_count = 0;
    for(h_count = 0; h_count < bmp_info.height; h_count++)
    {
        for(w_count = 0; w_count < bmp_info.width; w_count++)
        {
            read(fd_bmp, bmp_buf, sizeof(bmp_buf));
            lcd_buf[w_count + h_count * bmp_info.width] = bmp_buf[0] | bmp_buf[1] << 8 | bmp_buf[2] << 16;
        }
    }

    //将图片写入到lcd屏幕文件
    for(h_count = 0; h_count < bmp_info.height; h_count++)
    {
        for(w_count = 0; w_count < bmp_info.width; w_count++)
        {
            *(memp + w_count + h_count*var.xres) = lcd_buf[w_count + h_count * bmp_info.width];
        }
    }

    munmap(memp,var.xres*var.yres*var.bits_per_pixel/8);
    close(fd_bmp);
    close(fd_lcd);
    return 0;
}
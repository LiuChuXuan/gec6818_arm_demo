#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>


int main(void)
{
    int fd = open("/dev/fb0",O_RDWR);
    if(fd == -1)
    {
        perror("open");
        return -1;
    }

    struct fb_var_screeninfo var;
    ioctl(fd,FBIOGET_VSCREENINFO,&var);

    printf("x = %d, y = %d, pixel = %d\n",var.xres,var.yres,var.bits_per_pixel);

    int *memp = (int *)mmap(NULL,var.xres*var.yres*var.bits_per_pixel/8, 
                                PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);

    if(memp == MAP_FAILED)
    {
        perror("mmap");
        return -2;
    }

    //w = 500   h = 340
    int fd_bmp = open("a.bmp", O_RDONLY);
    if(fd_bmp == -1)
    {
        perror("open");
        return -1;
    }

    lseek(fd_bmp,  54,  SEEK_SET);
    
    char buf[3];
    memset(buf, 0, sizeof(buf));
    int w = 0;
    int h = 0;

    //int buf_[340][500];

    for(h = 0; h < 340; h++)
    {
        for(w = 0; w < 500; w++)
        {
            read(fd_bmp, buf, sizeof(buf));

            //1   2     3    4
            //      红  绿  蓝
            //*(memp + w + h*var.xres) = 0x000000ff;
            *(memp + w + h*var.xres) = buf[0] << 0  | buf[1] << 8 | buf[2] << 16;
        }
    }
/*
    int i = 0;
    for(i = 0; i < var.xres * var.yres; i++)
    {
        read(fd_bmp, buf, 3);

        *(memp+i) = buf[0] | buf[1] << 4 | buf[2] << 8;
    }
 */


    munmap(memp,var.xres*var.yres*var.bits_per_pixel/8);
    close(fd_bmp);
    close(fd);


    return 0;
}

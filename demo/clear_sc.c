#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

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

    int i = 0;
    for(i = 0;i < var.xres*var.yres; i++)
    {
        *(memp+i) = 0x00000000;
    }

    munmap(memp,var.xres*var.yres*var.bits_per_pixel/8);

    close(fd);


    return 0;
}

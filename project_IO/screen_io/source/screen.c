#include "screen.h"

int open_sc(sc_info_t screen)
{
    if (screen == NULL)
    {
        perror("open_sc == NULL");
        return -1;
    }
    int fd = open("/dev/fb0", O_RDWR);
    if (fd == -1)
    {
        perror("open");
        return -1;
    }

    struct fb_var_screeninfo var;
    ioctl(fd, FBIOGET_VSCREENINFO, &var);

    screen->xres = var.xres;
    screen->yres = var.yres;
    screen->bits_per_pixel = var.bits_per_pixel;
    screen->size = var.xres * var.yres * var.bits_per_pixel / 8;

    printf("x = %d, y = %d, pixel = %d\n",
           screen->xres, screen->yres, screen->bits_per_pixel);

    screen->fb = (int *)mmap(NULL, screen->size,
                             PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

    if (screen->fb == MAP_FAILED)
    {
        perror("mmap");
        close(fd);
        return -1;
    }

    /* 后面需要用
    munmap(screen->fb, screen->size);
释放资源 */

    close(fd);
    return 0;
}
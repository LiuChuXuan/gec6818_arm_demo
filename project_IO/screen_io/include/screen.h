#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <linux/fb.h>
#include <unistd.h>

typedef struct sc_info
{
    unsigned int *fb;   //帧缓存frame buffer
    int xres;           //宽
    int yres;           //高
    int bits_per_pixel; //每个像素占多少位
    int size;           //size = xres * yres * bits_per_pixel / 8
} sc_info, *sc_info_t;

int open_sc(sc_info_t screen);

#endif
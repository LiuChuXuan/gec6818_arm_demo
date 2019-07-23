#ifndef _SHOW_BMP_H_
#define _SHOW_BMP_H_
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

int show_bmp(char *path, int x, int y);

#endif
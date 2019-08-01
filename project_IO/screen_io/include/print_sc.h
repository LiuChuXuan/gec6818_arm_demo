#ifndef _PRINT_SC_H_
#define _PRINT_SC_H_

#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "input_sc.h"

int draw(unsigned int colour);
int draw_line(unsigned int colour, int pixel);
int draw_circle(unsigned int colour, int x_center, int y_center, int r);
static inline int circle_plot(unsigned int colour, int x_center, \
                int y_center, int x, int y, int *memp, int xres, int yres);

#endif
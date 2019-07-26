#ifndef _INPUT_SC_H_
#define _INPUT_SC_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>
#include <stdlib.h>

#define MOVE_LEFT        666666
#define MOVE_RIGHT     -666666
#define QUIT          115201

int get_xy(int *x0, int *y0, int *x1, int *y1);
int get_xy_release(int *x,int *y);
int ch_img(void);


#endif
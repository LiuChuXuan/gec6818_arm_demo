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

int get_xy_release(int *x,int *y);
int left_right(void);


#endif
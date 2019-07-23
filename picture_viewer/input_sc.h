#ifndef _INPUT_SC_H_
#define _INPUT_SC_H_

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>

int get_xy(int *x, int *y);
int left_right(void);


#endif
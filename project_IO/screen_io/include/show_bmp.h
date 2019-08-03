#ifndef _SHOW_BMP_H_
#define _SHOW_BMP_H_

#include "bit_bmp.h"
#include "input_sc.h"
#include "screen.h"

int clear_sc(sc_info_t screen);
int show_bmp(sc_info_t screen, char *path, int x, int y);
int show_bmp_over(sc_info_t screen, char *path, int x, int y);

#endif
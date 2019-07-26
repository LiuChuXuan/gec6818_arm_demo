#ifndef _IMSHOW_2048_H_
#define _IMSHOW_2048_H_

#include "input_sc.h"
#include "list_2048.h"
#include "show_bmp.h"

#define MOVE_ABOVE 999999
#define MOVE_BELOW -999999


int get_direction(void);
int imshow_2048(list_2048_t game);



#endif
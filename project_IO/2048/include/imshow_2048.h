#ifndef _IMSHOW_2048_H_
#define _IMSHOW_2048_H_

#include "input_sc.h"
#include "list_2048.h"


#define MOVE_ABOVE 999999
#define MOVE_BELOW -999999

int get_xy(int *x0, int *y0, int *x1, int *y1);
int get_direction(void);
int move(list_2048_t new_game);



#endif
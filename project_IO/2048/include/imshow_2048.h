#ifndef _IMSHOW_2048_H_
#define _IMSHOW_2048_H_

#include "input_sc.h"
#include "list_2048.h"
#include "show_bmp.h"

#ifndef MOVE_ABOVE
#define MOVE_ABOVE 999999
#endif
#ifndef MOVE_BELOW
#define MOVE_BELOW -999999
#endif
#ifndef MOVE_LEFT
#define MOVE_LEFT 666666
#endif
#ifndef MOVE_RIGHT
#define MOVE_RIGHT -666666
#endif
#ifndef QUIT
#define QUIT 115201
#endif
#ifndef GAME_2048_RESTART
#define GAME_2048_RESTART 123210
#endif
#ifndef GAME_2048_REVOKE
#define GAME_2048_REVOKE 6543210
#endif

int get_direction_2048(void);
int imshow_2048(sc_info_t screen, list_2048_t game);

#endif
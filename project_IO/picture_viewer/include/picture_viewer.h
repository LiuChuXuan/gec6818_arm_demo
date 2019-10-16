#ifndef _PICTURE_VIEWER_H_
#define _PICTURE_VIEWER_H_

#include "input_sc.h"
#include "scan_picture.h"
#include "list.h"
#include "show_bmp.h"

#ifndef MOVE_LEFT
#define MOVE_LEFT 666666
#endif
#ifndef MOVE_RIGHT
#define MOVE_RIGHT -666666
#endif
#ifndef QUIT
#define QUIT 115201
#endif

int picture_viewer(sc_info_t screen, char *path);
int ch_img(void);

#endif
#ifndef _PRINT_SC_H_
#define _PRINT_SC_H_

#include "screen.h"
#include "input_sc.h"

int draw(sc_info_t screen, unsigned int colour);

int draw_line(unsigned int colour, int pixel);

int draw_circle(sc_info_t screen, \
    unsigned int colour, int x_center, \
    int y_center, int r);

static inline int circle_plot(sc_info_t screen, \
                        unsigned int colour, \
                    int x_center, int y_center, \
                        int x, int y);

#endif
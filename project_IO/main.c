#include "main.h"


int main(void)
{
    int x,y;
    while(1)
    {
        show_bmp("./ui_img/menu.bmp", 0, 0);
        get_xy_release(&x, &y);
        //电子相册
        if(x < 400 && y < 240)
        {
            perror("pecture viewer");
            picture_viewer("./img");
        }
        if(x > 400 && y < 240)
        {
            perror("led beep");
            led_beep();
        }
        //2048
        else if(x < 400 && y >= 240)
        {
            perror("2048");
            game_2048();
        }
        else if(x > 400 && y >= 240)
        {
            clear_sc();
            perror("close");
            break;
        }
    }

    return 0;
}


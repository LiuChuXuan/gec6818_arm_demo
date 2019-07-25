#include "main.h"


int main(void)
{
    int x,y;
    while(1)
    {
        show_bmp("./ui_img/menu.bmp", 0, 0);
        get_xy_release(&x, &y);
        //电子相册
        if(x < 700 && y < 240)
        {
            picture_viewer("./img");
        }

        //2048
        else if(x < 700 && y >= 240)
        {
            game_2048();
        }
        else if(x > 700)
        {
            break;
        }
    }

    return 0;
}


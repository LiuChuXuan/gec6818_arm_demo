#include "main.h"


int main(void)
{
    sc_info_t screen = (sc_info_t)malloc(sizeof(sc_info));
    open_sc(screen);
    int x,y;
    while(1)
    {
        show_bmp(screen, "./ui_img/menu.bmp", 0, 0);
        get_xy_release(&x, &y);
        printf("(%d, %d)\n", x, y);
        //电子相册
        if(x > 579 && y > 69 && x < 730 && y < 152)
        {
            perror("pecture viewer");
            picture_viewer(screen, "./img");
        }
        //2048
        else if(x > 550 && y > 158 && x < 759 && y < 235)
        {
            perror("2048");
            game_2048(screen);
        }
        //控制LED灯的亮灭和蜂鸣器叫
        else if(x > 445 && y > 250 && x < 786 && y < 340)
        {
            perror("led beep");
            led_beep(screen);
        }
        //退出程序
        else if(x > 570 && y > 360 && x < 724 && y < 424)
        {
            clear_sc(screen);
            perror("close");
            break;
        }
    }

    munmap(screen->fb, screen->size);
    return 0;
}


#include "imshow_2048.h"

//接受触摸屏的点击事件，判断上下左右滑屏还是点击撤销、重来、退出
int get_direction_2048(void)
{
    int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
    unsigned int x_abs = 0, y_abs = 0;
    while((x0 == x1) && (y0 == y1))
    {
        get_xy(&x0, &y0, &x1, &y1);

        if(x0 > 565 && x1 >565 && 
        x0 < 691 && x1 < 691 &&
        y0 > 175 && y0 > 175 &&
        y0 < 250 && y1 < 250)
        {
            return GAME_2048_REVOKE;
        }
        if(x0 > 565 && x1 >565 && 
        x0 < 691 && x1 < 691 &&
        y0 > 275 && y0 > 275 &&
        y0 < 345 && y1 < 345)
        {
            return GAME_2048_RESTART;
        }
        if(x0 > 565 && x1 >565 && 
            x0 < 691 && x1 < 691 &&
            y0 > 368 && y0 > 368 &&
            y0 < 436 && y1 < 436)
        {
            return QUIT;
        }

        x_abs = ((x0 - x1) >= 0) ? (x0 - x1) : (-(x0 - x1));
        y_abs = ((y0 - y1) >= 0) ? (y0 - y1) : (-(y0 - y1));
        if(x_abs >= y_abs)
        {
            if(x0 > x1)
            {
                return MOVE_LEFT;
            }
            else if(x0 < x1)
            {
                return MOVE_RIGHT;
            }
        }
        else
        {
            if(y0 > y1)
            {
                return MOVE_ABOVE;
            }
            else if(y0 < y1)
            {
                return MOVE_BELOW;
            }
        }
    }
}

//2048显示图片的函数
int imshow_2048(sc_info_t screen, list_2048_t game)
{
    if(game == NULL)
    {
        perror("imshow game==NULL");
        return -1;
    }
    show_bmp(screen, "./ui_img/menu2048.bmp", 0, 0);

    int x = 0, y = 0;
    int w = 0, h = 0;

    for(y = 0; y < 4; y++)
    {
        h = y * 90 + 70;
        for(x = 0; x < 4; x++)
        {
            w = x * 90 + 70;
            switch(game->item[y][x])
            {
                case 0:
                    show_bmp(screen, "./ui_img/item0.bmp", w, h);
                    break;
                case 2:
                    show_bmp(screen, "./ui_img/item2.bmp", w, h);
                    break;
                case 4:
                    show_bmp(screen, "./ui_img/item4.bmp", w, h);
                    break;
                case 8:
                    show_bmp(screen, "./ui_img/item8.bmp", w, h);
                    break;
                case 16:
                    show_bmp(screen, "./ui_img/item16.bmp", w, h);
                    break;
                case 32:
                    show_bmp(screen, "./ui_img/item32.bmp", w, h);
                    break;
                case 64:
                    show_bmp(screen, "./ui_img/item64.bmp", w, h);
                    break;
                case 128:
                    show_bmp(screen, "./ui_img/item128.bmp", w, h);
                    break;
                case 256:
                    show_bmp(screen, "./ui_img/item256.bmp", w, h);
                    break;
                case 512:
                    show_bmp(screen, "./ui_img/item512.bmp", w, h);
                    break;
                case 1024:
                    show_bmp(screen, "./ui_img/item1024.bmp", w, h);
                    break;
                case 2048:
                    show_bmp(screen, "./ui_img/item2048.bmp", w, h);
                    break;
                case 4096:
                    show_bmp(screen, "./ui_img/item4096.bmp", w, h);
                    break;
                case 8192:
                    show_bmp(screen, "./ui_img/item8192.bmp", w, h);
                    break;
                default:
                    printf("value over 2048\n");
                    break;
            }
        }
    }
    w = 665;

    if(game->score >= 10)
    {
        int count = 0;
        for(count = 0; ; count++)
        {
            y = 1;
            for(x = 0; x < count; x++)
            {
                y *= 10;
            }
            switch((game->score/y) % 10)
            {
                case 0:
                    show_bmp(screen, "./ui_img/score0.bmp", w, 105);
                    break;
                case 1:
                    show_bmp(screen, "./ui_img/score1.bmp", w, 105);
                    break;
                case 2:
                    show_bmp(screen, "./ui_img/score2.bmp", w, 105);
                    break;
                case 3:
                    show_bmp(screen, "./ui_img/score3.bmp", w, 105);
                    break;
                case 4:
                    show_bmp(screen, "./ui_img/score4.bmp", w, 105);
                    break;
                case 5:
                    show_bmp(screen, "./ui_img/score5.bmp", w, 105);
                    break;
                case 6:
                    show_bmp(screen, "./ui_img/score6.bmp", w, 105);
                    break;
                case 7:
                    show_bmp(screen, "./ui_img/score7.bmp", w, 105);
                    break;
                case 8:
                    show_bmp(screen, "./ui_img/score8.bmp", w, 105);
                    break;
                case 9:
                    show_bmp(screen, "./ui_img/score9.bmp", w, 105);
                    break;
                default:
                    perror("score error");
                    return(-1);
            }
            if(game->score/(y*10) == 0)
            {
                break;
            }
            w = w - 25;
        }
    }
    else
    {
        switch(game->score)
        {
            case 0:
                show_bmp(screen, "./ui_img/score0.bmp", w, 105);
                break;
            case 1:
                show_bmp(screen, "./ui_img/score1.bmp", w, 105);
                break;
            case 2:
                show_bmp(screen, "./ui_img/score2.bmp", w, 105);
                break;
            case 3:
                show_bmp(screen, "./ui_img/score3.bmp", w, 105);
                break;
            case 4:
                show_bmp(screen, "./ui_img/score4.bmp", w, 105);
                break;
            case 5:
                show_bmp(screen, "./ui_img/score5.bmp", w, 105);
                break;
            case 6:
                show_bmp(screen, "./ui_img/score6.bmp", w, 105);
                break;
            case 7:
                show_bmp(screen, "./ui_img/score7.bmp", w, 105);
                break;
            case 8:
                show_bmp(screen, "./ui_img/score8.bmp", w, 105);
                break;
            case 9:
                show_bmp(screen, "./ui_img/score9.bmp", w, 105);
                break;
            default:
                perror("score error");
                return(-1);
        }
    }
    return 0;
}


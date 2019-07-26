#include "imshow_2048.h"

//判断上下左右
int get_direction(void)
{
    int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
    unsigned int x_abs = 0, y_abs = 0;
    while((x0 == x1) && (y0 == y1))
    {
        get_xy(&x0, &y0, &x1, &y1);
        if(x1 > 700 && x0 >700 && y0 > 380 && y0 >380)
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


int imshow_2048(list_2048_t game)
{
    if(game == NULL)
    {
        perror("imshow game==NULL");
        return -1;
    }
    show_bmp("./ui_img/menu2048.bmp", 0, 0);

    int x = 0, y = 0;
    int w = 0, h = 0;

    for(y = 0; y < 4; y++)
    {
        h = y * 120;
        for(x = 0; x < 4; x++)
        {
            w = x * 175;
            switch(game->item[y][x])
            {
                case 0:
                    show_bmp("./ui_img/item0.bmp", w, h);
                    break;
                case 2:
                    show_bmp("./ui_img/item2.bmp", w, h);
                    break;
                case 4:
                    show_bmp("./ui_img/item4.bmp", w, h);
                    break;
                case 8:
                    show_bmp("./ui_img/item8.bmp", w, h);
                    break;
                case 16:
                    show_bmp("./ui_img/item16.bmp", w, h);
                    break;
                case 32:
                    show_bmp("./ui_img/item32.bmp", w, h);
                    break;
                case 64:
                    show_bmp("./ui_img/item64.bmp", w, h);
                    break;
                case 128:
                    show_bmp("./ui_img/item128.bmp", w, h);
                    break;
                case 246:
                    show_bmp("./ui_img/item246.bmp", w, h);
                    break;
                case 512:
                    show_bmp("./ui_img/item512.bmp", w, h);
                    break;
                case 1024:
                    show_bmp("./ui_img/item1024.bmp", w, h);
                    break;
                case 2048:
                    show_bmp("./ui_img/item2048.bmp", w, h);
                    break;
                default:
                    printf("value over 2048\n");
                    break;
            }
        }
    }
    w = 778;

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
                    show_bmp("./ui_img/score0.bmp", w, 80);
                    break;
                case 1:
                    show_bmp("./ui_img/score1.bmp", w, 80);
                    break;
                case 2:
                    show_bmp("./ui_img/score2.bmp", w, 80);
                    break;
                case 3:
                    show_bmp("./ui_img/score3.bmp", w, 80);
                    break;
                case 4:
                    show_bmp("./ui_img/score4.bmp", w, 80);
                    break;
                case 5:
                    show_bmp("./ui_img/score5.bmp", w, 80);
                    break;
                case 6:
                    show_bmp("./ui_img/score6.bmp", w, 80);
                    break;
                case 7:
                    show_bmp("./ui_img/score7.bmp", w, 80);
                    break;
                case 8:
                    show_bmp("./ui_img/score8.bmp", w, 80);
                    break;
                case 9:
                    show_bmp("./ui_img/score9.bmp", w, 80);
                    break;
                default:
                    perror("score error");
                    exit(-1);
            }
            if(game->score/(y*10) == 0)
            {
                break;
            }
            w = w - 19;
        }
    }
    else
    {
        switch(game->score)
        {
            case 0:
                show_bmp("./ui_img/score0.bmp", w, 80);
                break;
            case 1:
                show_bmp("./ui_img/score1.bmp", w, 80);
                break;
            case 2:
                show_bmp("./ui_img/score2.bmp", w, 80);
                break;
            case 3:
                show_bmp("./ui_img/score3.bmp", w, 80);
                break;
            case 4:
                show_bmp("./ui_img/score4.bmp", w, 80);
                break;
            case 5:
                show_bmp("./ui_img/score5.bmp", w, 80);
                break;
            case 6:
                show_bmp("./ui_img/score6.bmp", w, 80);
                break;
            case 7:
                show_bmp("./ui_img/score7.bmp", w, 80);
                break;
            case 8:
                show_bmp("./ui_img/score8.bmp", w, 80);
                break;
            case 9:
                show_bmp("./ui_img/score9.bmp", w, 80);
                break;
            default:
                perror("score error");
                exit(-1);
        }
    }
    return 0;
}


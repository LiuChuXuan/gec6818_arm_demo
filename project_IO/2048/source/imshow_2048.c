#include "imshow_2048.h"

//获取按下时的x0,y0和松手时的x1,y1
int get_xy(int *x0, int *y0, int *x1, int *y1)
{
    struct input_event evt;
	memset(&evt, 0, sizeof(evt));

    int fd = open("/dev/input/event0",O_RDWR);
    if(fd == -1)
	{
        perror("open event0 failed");
        return -1;
    }

	while(read(fd,&evt,sizeof(evt))>0)
	{
		//不断的获取坐标
        if((evt.type==EV_ABS))
		{
			if(evt.code == ABS_X)
			{
                *x1 = evt.value;
			}
			else if(evt.code == ABS_Y)
			{
                *y1 = evt.value;
			}
		}

		else if((evt.type == EV_KEY) && (evt.code == BTN_TOUCH))
		{
            if(evt.value == 1)
            {
                *x0 = *x1;
                *y0 = *y1;
            }
			if(evt.value == 0)
			{
				break;
			}
		}
    }
    return 0;
}

//判断上下左右
int get_direction(void)
{
    int x0 = 0, y0 = 0, x1 = 0, y1 = 0;
    unsigned int x_abs = 0, y_abs = 0;
    while((x0 == x1) && (y0 == y1))
    {
        get_xy(&x0, &y0, &x1, &y1);

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

int move(list_2048_t game)
{
    int direction = 0, score = 0;
    while(1)
    {
        direction = get_direction();
        switch(direction)
        {
            case MOVE_ABOVE: 
                score += list_2048_move_above(game);
                list_2048_randcreate(game);
                printf("above, score = %d\n", score);
                list_2048_travel(game);
                imshow_2048(game);
                break;
            case MOVE_BELOW:
                score += list_2048_move_below(game);
                list_2048_randcreate(game);
                printf("below, score = %d\n", score);
                list_2048_travel(game);
                imshow_2048(game);
                break;
            case MOVE_LEFT:
                score += list_2048_move_left(game);
                list_2048_randcreate(game);
                printf("left, score = %d\n", score);
                list_2048_travel(game);
                imshow_2048(game);
                break;
            case MOVE_RIGHT:
                score += list_2048_move_right(game);
                list_2048_randcreate(game);
                printf("right, score = %d\n", score);
                list_2048_travel(game);
                imshow_2048(game);
                break;
            default:
                printf("move while switch default");
                break;
        }
    }
    return score;
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
    return 0;
}


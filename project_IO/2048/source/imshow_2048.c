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

int move(list_2048_t new_game)
{
    int direction = 0, score = 0;
    while(1)
    {
        direction = get_direction();
        switch(direction)
        {
            case MOVE_ABOVE: 
                score += list_2048_move_above(new_game);
                list_2048_randcreate(new_game);
                printf("above, score = %d\n", score);
                list_2048_travel(new_game);
                break;
            case MOVE_BELOW:
                score += list_2048_move_below(new_game);
                list_2048_randcreate(new_game);
                printf("below, score = %d\n", score);
                list_2048_travel(new_game);
                break;
            case MOVE_LEFT:
                score += list_2048_move_left(new_game);
                list_2048_randcreate(new_game);
                printf("left, score = %d\n", score);
                list_2048_travel(new_game);
                break;
            case MOVE_RIGHT:
                score += list_2048_move_right(new_game);
                list_2048_randcreate(new_game);
                printf("right, score = %d\n", score);
                list_2048_travel(new_game);
                break;
            default:
                printf("move while switch default");
                break;
        }
    }
    return score;
}
#include "input_sc.h"

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

//获取松开手时的(x,y)
int get_xy_release(int *x,int *y)
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
                *x = evt.value;
			}
			else if(evt.code == ABS_Y)
			{
                *y = evt.value;
			}
		}

		else if((evt.type == EV_KEY) && (evt.code == BTN_TOUCH))
		{
			if(evt.value == 0)
			{
				break;
			}
		}
    }

    close(fd);
	return 0;
}

//切换图片
int ch_img(void)
{
	int x0 = 0, y0 = 0, x1 = 0, y1 = 0;

	int ret = get_xy(&x0, &y0, &x1, &y1);
	if(ret != 0)
	{
		perror("ch_img get xy error!");
		return -1;
	}

	printf("(%d,%d)\n",x0,x1);

	if((x0 < 15) && (x1 > 50))
	{
		printf("quit l : (%d,%d)\n",x0,x1);
		return QUIT;
	}
	else if((x0 > 785) && (x1 < 750))
	{
		printf("quit r : (%d,%d)\n",x0,x1);
		return QUIT;
	}
	else
	{
		if(x0 > x1)
		{
			return MOVE_LEFT;
		}
		else if(x0 < x1)
		{
			return MOVE_RIGHT;
		}
		else
		{
			return x0;
		}
	}
}
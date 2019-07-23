#include "input_sc.h"


//向左划返回MOVE_LEFT(666666)
//向右划返回MOVE_RIGHT(999999)
int left_right(void)
{
    int x = 0;
	int x0 = 0, x1 = 0;
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
        //处理坐标事件
        if((evt.type==EV_ABS) && (evt.code==ABS_X))
		{
                x = evt.value;
		}

		else if((evt.type == EV_KEY) && (evt.code == BTN_TOUCH))
		{
			if(evt.value == 1)
			{
				x0 = x;
			}
			else if(evt.value == 0)
			{
				x1 = x;
				break;
			}
		}
    }

    close(fd);

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
		return x;
	}
}
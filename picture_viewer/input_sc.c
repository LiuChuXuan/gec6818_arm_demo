#include "input_sc.h"

//获取触摸位置
int get_xy(int *x, int *y)
{

	int  pressure;
	//1、打开文件"/dev/input/event0"
	int ts_fd = open("/dev/input/event0", O_RDWR);
	if(ts_fd == -1)
	{
		perror("open event0 faild!\n");
		return -1;
	}
	int flag1=1, flag2=1;
	struct input_event ts_buf;
    memset(&ts_buf, 0, sizeof(ts_buf));
	//不断的读取
	while(1)
	{
		//2、阻塞等待触摸，读取触摸屏文件里面的数据
		read(ts_fd, &ts_buf, sizeof(ts_buf));
		
		//3、分析处理触摸屏数据
		//判断是否为触摸屏数据
		if(ts_buf.type == EV_ABS)
		{
			//进入判断事件类型是否为x轴事件
			if(ts_buf.code == ABS_X && flag1)
			{
				*x = ts_buf.value;
				flag1 = 0;
			}
			
			//进入判断事件类型是否为Y轴事件
			else if(ts_buf.code == ABS_Y  && flag2)
			{
				*y = ts_buf.value;
				flag2 = 0;
			}
			
			if(flag1==0 && flag2 == 0)
			{
				printf("y:%d\n", *y);
				printf("x:%d\n", *x);
				break;
			}
			
			/* //进入判断事件类型是否为压力值事件
			else if(ts_buf.code == ABS_PRESSURE  )
			{
				pressure = ts_buf.value;
				printf("pressure:%d\n", pressure);
			} */
			
		}
		
		//判断是否为按键事件数据,手指离开屏幕，压力值为0
		else if(ts_buf.type == EV_KEY && ts_buf.code == BTN_TOUCH && ts_buf.value == 0)
		{
				printf("y:%d\n", *y);
				printf("x:%d\n", *x);
				break;
				//pressure = ts_buf.value;
				//printf("pressure1:%d\n", pressure);
			
		}
	}
	//4、关闭文件
	close(ts_fd);
	return 0;
}

//判断左划还是右划，左划返回0，右划返回1
int left_right(void)
{
    struct input_event evt;/*  /dev/input/event0  */  
   
    //打开输入设备文件
    int fd = open("/dev/input/event0",O_RDWR);
    if(fd==-1)
    {
        perror("open");
        return -1;
    }

    printf("fd = %d", fd);

    int flag = 0;//0x00000000  最后四位从左到右 x1 x2 y1 y2
    int x1 = 0, x2 = 0;
    int y1 = 0, y2 = 0;
    while(read(fd,&evt,sizeof(evt))>0)
    {
        if(evt.type==EV_ABS)
        {
            if(evt.code==ABS_X)
            {
                if((flag & 0x00001000) == 0x00001000)
                {
                    x1 = evt.value;
                    flag |= 0x00001000;
                }
                x2 = evt.value;
            }
            else if(evt.code == ABS_Y)
            {
                if((flag & 0x00000010) == 0x00000010)
                {
                    y1 = evt.value;
                    flag |= 0x00000010;
                }
                y2 = evt.value;
            }
        }

        if(evt.type==EV_KEY&&evt.value==1)
        {
            printf("(%d,%d)",x2,y2);
        }
    }
    printf("松开时的坐标为(%d,%d)\n",x2,y2);
    printf("按下时的坐标为(%d,%d)\n",x1,y1);

    if(x2 > x1)
    {
        close(fd);
        return 1;
    }
    
    if(x1 > x2)
    {
        close(fd);
        return 0;
    }


    return -1;
}
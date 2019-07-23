#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h> 
#include <linux/input.h>


int main(int argc, char **argv)
{
	int x, y, pressure;
	//1、打开文件"/dev/input/event0"
	int ts_fd = open("/dev/input/event0", O_RDWR);
	if(ts_fd == -1)
	{
		perror("open event0 faild!\n");
		return -1;
	}
	
	struct input_event ts_buf;
	bzero(&ts_buf, sizeof(ts_buf));
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
			if(ts_buf.code == ABS_X)
			{
				x = ts_buf.value;
				printf("x:%d\n", x);
			}
			
			//进入判断事件类型是否为Y轴事件
			else if(ts_buf.code == ABS_Y)
			{
				y = ts_buf.value;
				printf("y:%d\n", y);
			}
			
			//进入判断事件类型是否为压力值事件
			else if(ts_buf.code == ABS_PRESSURE)
			{
				pressure = ts_buf.value;
				printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
				printf("pressure:%d\n", pressure);
			}
			
		}
		
		//判断是否为按键事件数据
		else if(ts_buf.type == EV_KEY)
		{
			//进入判断事件类型是否为BTN_TOUCH事件
			if(ts_buf.code == BTN_TOUCH)
			{
				pressure = ts_buf.value;
				printf("pressure1:%d\n", pressure);
			}
		
		}
	}
	//4、关闭文件
	close(ts_fd);
	
	return 0;
}
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h> 
#include <linux/input.h>
#include <sys/mman.h>
int show_bmp(int x, int y, int bmp_w, int bmp_h, char *bmp_name);
int get_xy();


int main(int argc, char **argv)
{
	int x, y; 
	int i;
	while(1)
	{
		get_xy(&x, &y);
		i++;
		printf("i:%d-->x, y: %d\t %d\n", i, x, y);
		if(400>x && x>200 && y>100 && y<400)
		{
			show_bmp(0, 0, 800, 480, "./3.bmp");
		}
		if(700>x && x>500 && y>100 && y<400)
		{
			show_bmp(0, 0, 800, 480, "./4.bmp");
		}
	}
	return 0;
}

int get_xy(int *x, int *y)
{
	int flag1=1, flag2=1;
	struct input_event ts_buf;
	bzero(&ts_buf, sizeof(ts_buf));
	int  pressure;
	//1、打开文件"/dev/input/event0"
	int ts_fd = open("/dev/input/event0", O_RDWR);
	if(ts_fd == -1)
	{
		perror("open event0 faild!\n");
		return -1;
	}

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



int show_bmp(int x, int y, int bmp_w, int bmp_h, char *bmp_name)
{
	if(x+bmp_w > 800 || y+bmp_h > 480)
	{
		printf("bmp too big !\n");
		return -1;
	}
	
	
	//1、打开bmp文件400*300
	FILE *bmp_fp = fopen(bmp_name, "r");
	if(bmp_fp ==NULL)
	{
		perror("fopen bmp failed!");
		printf("fopen %s failed!\n", bmp_name);	
		return -1;
	}
	
	printf("showing %s\n", bmp_name);
	
	//	2、跳过54个字节文件头
	int result = fseek(bmp_fp, 54, SEEK_SET);
//	printf("%d\n",result);

	//因为地址对齐多出来的字节数
	int ret = (4-bmp_w*3%4)%4; //每行多补的字节数
		
	
	unsigned char bmp_buf[bmp_w*bmp_h*3+ret*bmp_h];
	bzero(bmp_buf, bmp_w*bmp_h*3+ret*bmp_h);
	
	//	3、读取bmp图片的像素点数据
	fread(bmp_buf, bmp_w*bmp_h*3+ret*bmp_h, 1, bmp_fp);
		
	//	4、关闭bmp文件
	fclose(bmp_fp);
	
	unsigned int lcd_buf[bmp_w*bmp_h];//bmp_w*bmp_h*4 
	bzero(lcd_buf, bmp_w*bmp_h*4);
	
	//	5、合成LCD屏幕类型像素点	
	//(1)合成300排像素点
	int i, j;
	
	for(j=0; j<bmp_h; j++)
	{
		for(i=0; i<bmp_w; i++)
		{
			//	ret*j ，前面多出来的字节数
			lcd_buf[i+j*bmp_w] = bmp_buf[3*i+(bmp_w*3+ret)*j+0] ;
			lcd_buf[i+j*bmp_w] |= bmp_buf[3*i+bmp_w*3*j+ret*j+1]<<8;
			lcd_buf[i+j*bmp_w] |= bmp_buf[3*i+bmp_w*3*j+ret*j+2]<<16;
		}
	}
	//	6、打开LCD屏幕文件
	int lcd_fd = open("/dev/fb0", O_RDWR);
	if(lcd_fd == -1)
	{
		perror("open fb0 failed!\n");
		return -1;
	}
	
	//	7、内存映射
	unsigned int *addr = (unsigned int *)mmap(NULL, 800*480*4, PROT_READ | PROT_WRITE, MAP_SHARED, lcd_fd, 0);
	if(addr == MAP_FAILED)
	{
		perror("mmap failed!");
		close(lcd_fd);
		return -1;
	}
	
	//	8、把数据通过指针赋值的方式写入文件 从下往上
	for(j=bmp_h-1; j>=0; j--)  //299
	{
		for(i=0; i<bmp_w; i++)
		{
		
			//											300-1-300 == -1
			*(addr+i+800*j+y*800+x) = lcd_buf[i+bmp_w*(bmp_h-1-j)];//任意位置，正立
			
		}
		//usleep(100);//延时10毫秒
	}
		
		
	//	9、关闭文件和解除映射
	close(lcd_fd);
	munmap(addr, 800*480*4);	
	
	return 0;
	
}

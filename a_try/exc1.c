#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>   
#include <sys/mman.h>


int show_bmp(int x, int y, int bmp_w, int bmp_h, char *bmp_name);

int main(int argc, char **argv)
{
	
	//show_bmp(0, 0, 400, 300, "/193/2.bmp");
	
	
	show_bmp(200, 100, 402, 300, argv[1]);
	
	return 0;
}


int show_bmp(int x, int y, int bmp_w, int bmp_h, char *bmp_name)
{
	//1、打开bmp文件400*300
	FILE *bmp_fp = fopen(bmp_name, "r");
	if(bmp_fp ==NULL)
	{
		perror("fopen bmp failed!");
		printf("fopen %s failed!\n", bmp_name);	
		return -1;
	}
		
	//	2、跳过54个字节文件头
	fseek(bmp_fp, 54, SEEK_SET);
		
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
	
	//	8、把数据通过指针赋值的方式写入文件
	for(j=0; j<bmp_h; j++)
	{
		for(i=0; i<bmp_w; i++)
		{
			//			0						0  -->299  (300-1-0)
			//			1						1  -->298   (300-1-1)
			
			//          0							299
			//			1							298
			*(addr+i+800*j+y*800+x) = lcd_buf[i+bmp_w*(bmp_h-1-j)];//任意位置，正立
			//*(addr+i+800*j) = lcd_buf[i+bmp_w*(bmp_h-1-j)];//原点，正立
			//	*(addr+i+800*j) = lcd_buf[i+bmp_w*j]; //原点，倒立
		}
		
		usleep(10*1000);//延时10毫秒
	}
		
		
	//	9、关闭文件和解除映射
	close(lcd_fd);
	munmap(addr, 800*480*4);	
	
	return 0;
	
}



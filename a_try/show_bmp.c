#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>   
#include <sys/mman.h>



int main(int argc, char *argv[])
{
	
	char *bmp_name = argv[1];
	
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
		
	unsigned char bmp_buf[400*300*3];
	bzero(bmp_buf, 400*300*3);
	
	//	3、读取bmp图片的像素点数据
	fread(bmp_buf, 400*300*3, 1, bmp_fp);
		
	//	4、关闭bmp文件
	fclose(bmp_fp);
	
	unsigned int lcd_buf[400*300];//400*300*4
	bzero(lcd_buf, 400*300*4);
	
	//	5、合成LCD屏幕类型像素点
	
	/* //(1)合成一个像素点
	
	
	lcd_buf[0] = bmp_buf[0] ;
	lcd_buf[0] |= bmp_buf[1]<<8;
	lcd_buf[0] |= bmp_buf[2]<<16;
		
		//lcd_buf[0] = bmp_buf[0] | bmp_buf[1]<<8 | bmp_buf[2]<<16;
	
	//(1)合成一排像素点
	int i;
	for(i=0; i<400; i++)
	{
		// i=0,     012
		//i=1       345
		//i=2		678
		lcd_buf[i] = bmp_buf[3*i+0] ;
		lcd_buf[i] |= bmp_buf[3*i+1]<<8;
		lcd_buf[i] |= bmp_buf[3*i+2]<<16;
		
	} */
	//(1)合成300排像素点
	int i, j;
	
	for(j=0; j<300; j++)
	{
		for(i=0; i<400; i++)
		{
			//j=0, i=0,  0        012
			//j=1, i=0	 400		1200, 1201, 1202
	
			lcd_buf[i+j*400] = bmp_buf[3*i+400*3*j+0] ;
			lcd_buf[i+j*400] |= bmp_buf[3*i+400*3*j+1]<<8;
			lcd_buf[i+j*400] |= bmp_buf[3*i+400*3*j+2]<<16;
			
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
	for(j=0; j<300; j++)
	{
		for(i=0; i<400; i++)
		{
	//j=1, i=0    800			400
	//j=2, i=0		1600		800
			*(addr+i+800*j) = lcd_buf[i+400*j];
		}
	}
		
		
	//	9、关闭文件和解除映射
	close(lcd_fd);
	munmap(addr, 800*480*4);
			
	
	return 0;
}






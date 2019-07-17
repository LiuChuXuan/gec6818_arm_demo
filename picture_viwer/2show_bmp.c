#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>


int main(void)
{
    //打开LCD屏幕文件
    int fd = open("/dev/fb0",O_RDWR);
    if(fd == -1)
    {
        perror("open");
        return -1;
    }

    struct fb_var_screeninfo var;
    ioctl(fd,FBIOGET_VSCREENINFO,&var);

    printf("x = %d, y = %d, pixel = %d\n",var.xres,var.yres,var.bits_per_pixel);

    int *memp = (int *)mmap(NULL,var.xres*var.yres*var.bits_per_pixel/8, 
                                PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);

    if(memp == MAP_FAILED)
    {
        perror("mmap");
        return -2;
    }
	char *bmp_name = "a.bmp";
	
	//1、打开bmp文件400*300
	FILE *bmp_fp = fopen(bmp_name, "r");
	if(bmp_fp ==NULL)
	{
		perror("fopen bmp failed!");
		printf("fopen %s failed!\n", bmp_name);	
		return -1;
	}
	unsigned char bmp_buf[500*340*3];

	//	2、跳过54个字节文件头
	fseek(bmp_fp, 54, SEEK_SET);

    	//	3、读取bmp图片的像素点数据
	fread(bmp_buf, 500*340*3, 1, bmp_fp);

    	//	4、关闭bmp文件
	fclose(bmp_fp);

    unsigned int lcd_buf[500*340];//400*300*4
    memset(lcd_buf, 0, sizeof(lcd_buf));

    int w = 0, h = 0;
    for(h = 0; h < 340; h++)
    {
        for(w = 0; w < 500; w++)
        {
            /*
            lcd_buf[w+h*500] = bmp_buf[3*w+500*3*h+0] ;
			lcd_buf[w+h*500] |= bmp_buf[3*w+500*3*h+1]<<8;
			lcd_buf[w+h*500] |= bmp_buf[3*w+500*3*h+2]<<16;
             */
            lcd_buf[w+h*500] = bmp_buf[3*w+500*3*h+0] | bmp_buf[3*w+500*3*h+1]<<8 | bmp_buf[3*w+500*3*h+2]<<16;

           //read(fd_bmp, buf, sizeof(buf));
            //1   2     3    4
            //      红  绿  蓝
            //*(memp + w + h*var.xres) = 0x000000ff;
            //*(memp + w + h*var.xres) = buf[0] << 0  | buf[1] << 8 | buf[2] << 16;
        }
    }

    for(h = 0; h < 340; h++)
    {
        for(w = 0; w < 500; w++)
        {
            *(memp + w + h*var.xres) = lcd_buf[w+500*h];
        }
    }
/*
    int i = 0;
    for(i = 0;i < var.xres*var.yres; i++)
    {
        *(memp+i) = 0xff0000;
    } 
*/

    munmap(memp,var.xres*var.yres*var.bits_per_pixel/8);
    close(fd);
    return 0;
}

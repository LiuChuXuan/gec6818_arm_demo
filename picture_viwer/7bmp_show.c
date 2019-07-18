#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "bit_bmp.h"

int main(int argc, char *argv[])
{
    int ret = 0;
    struct bitmap_header bmp_header;
    struct bitmap_info bmp_info;
    memset(&bmp_header, 0, 14);
    memset(&bmp_info, 0, 40);

    if(argc != 2)
    {
        printf("argc != 2");
        return -1;
    }

    FILE *fp_bmp = fopen(argv[1], "r");
    if(fp_bmp == NULL)
    {
        perror("open failed");
        return -1;
    }
    
    ret = fread(&bmp_header,  14, 1, fp_bmp);
    if(ret != 1)
    {
        perror("read error 14");
    }
    //printf("%d\n",bmp_header.type);
    //如果不是bmp格式则退出程序。
    if(bmp_header.type != 19778)
    {
        printf("It's not bmp file");
        return -1;
    }

    ret = fread(&bmp_info,  40, 1, fp_bmp);
    if(ret != 1)
    {
        perror("read error 40");
    }
    
    printf("bmp_x = %d, bmp_y =  %d\n", bmp_info.width, bmp_info.height);

    //因为地址对齐多出来的字节数
	ret = (4-bmp_info.width*3%4)%4; //每行多补的字节数
    unsigned char bmp_buf[bmp_info.height * bmp_info.width * 3 + ret * bmp_info.width];
    memset(bmp_buf, 0, sizeof(bmp_buf));

    //读取bmp图片到bmp_buf
	fread(bmp_buf, sizeof(bmp_buf), 1, fp_bmp);

    int fd_lcd = open("/dev/fb0",O_RDWR);
    if(fd_lcd == -1)
    {
        perror("open");
        return -1;
    }

    //打开lcd设备
    struct fb_var_screeninfo var;
    ioctl(fd_lcd,FBIOGET_VSCREENINFO,&var);

    //内存映射
    unsigned int *memp = (unsigned int *)mmap(NULL,var.xres*var.yres*var.bits_per_pixel/8, 
                                PROT_WRITE|PROT_READ,MAP_SHARED,fd_lcd,0);

    if(memp == MAP_FAILED)
    {
        perror("mmap");
        return -2;
    }

    printf("lcd_x = %d, lcd_y = %d, pixel = %d\n", var.xres, var.yres, var.bits_per_pixel);
    
    unsigned int lcd_buf[ bmp_info.width * bmp_info.height];
    memset(lcd_buf, 0, sizeof(lcd_buf));

    //将图片读取到bmp_buf再整合到lcd_buf中
	int i, j,w_count,h_count;
	for(j=0; j<bmp_info.height; j++)
	{
		for(i=0; i<bmp_info.width; i++)
		{
			//	ret*j ，前面多出来的字节数
			lcd_buf[i+j*bmp_info.width] = bmp_buf[3*i+(bmp_info.width*3+ret)*j+0] ;
			lcd_buf[i+j*bmp_info.width] |= bmp_buf[3*i+bmp_info.width*3*j+ret*j+1]<<8;
			lcd_buf[i+j*bmp_info.width] |= bmp_buf[3*i+bmp_info.width*3*j+ret*j+2]<<16;
		}
	}

    //偏移x,y
    unsigned int x = 100, y = 100;

    //将图片写入到lcd屏幕文件
    for(h_count = 0; h_count < bmp_info.height; h_count++)
    {
        for(w_count = 0; w_count < bmp_info.width; w_count++)
        {
            *(memp + (w_count+x) + (h_count+y)*var.xres) = 
                            lcd_buf[w_count + (bmp_info.height - h_count - 1) * bmp_info.width];
        }
    }

    munmap(memp,var.xres*var.yres*var.bits_per_pixel/8);
    fclose(fp_bmp);
    close(fd_lcd);
    return 0;
}
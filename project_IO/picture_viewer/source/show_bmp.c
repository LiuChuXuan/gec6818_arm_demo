#include "show_bmp.h"

int clear_sc(void)
{
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

    int i = 0;
    for(i = 0;i < var.xres*var.yres; i++)
    {
        *(memp+i) = 0x00000000;
    }

    munmap(memp,var.xres*var.yres*var.bits_per_pixel/8);

    close(fd);
    return 0;
}

//输入图片的路径、左上角位置(x，y)，在LCD相应位置显示图片。
int show_bmp(char *path, int x, int y)
{
    int ret = 0;
    struct bitmap_header bmp_header;
    struct bitmap_info bmp_info;
    memset(&bmp_header, 0, 14);
    memset(&bmp_info, 0, 40);

    //打开BMP图片
    FILE *fp_bmp = fopen(path, "r");
    if(fp_bmp == NULL)
    {
        perror("open bmp failed");
        return -1;
    }

    //读取bmp头
    ret = fread(&bmp_header,  14, 1, fp_bmp);
    if(ret != 1)
    {
        perror("read error 14");
        return -1;
    }

    //如果不是bmp格式则返回-1
    if(bmp_header.type != 19778)
    {
        printf("It's not bmp file");
        return -1;
    }

    //读取bmp图片信息，获取图片宽度和高度
    ret = fread(&bmp_info,  40, 1, fp_bmp);
    if(ret != 1)
    {
        perror("read error 40");
    }
    //printf("bmp_x = %d, bmp_y =  %d\n", bmp_info.width, bmp_info.height);

    //因为地址对齐，每行需要多补的字节数
	ret = (4-bmp_info.width*3%4)%4; 
    
    //读取图片的缓存空间，大小为图片长*宽+每行对齐多出来的字节数
    unsigned char bmp_buf[bmp_info.height * bmp_info.width * 3 + ret * bmp_info.height];
    memset(bmp_buf, 0, sizeof(bmp_buf));

    //读取bmp图片到bmp_buf
	fread(bmp_buf, sizeof(bmp_buf), 1, fp_bmp);
    
    //打开lcd设备文件
    int fd_lcd = open("/dev/fb0",O_RDWR);
    if(fd_lcd == -1)
    {
        perror("open");
        return -1;
    }
    struct fb_var_screeninfo var;//用于存放屏幕设备的宽度、高度信息
    ioctl(fd_lcd,FBIOGET_VSCREENINFO,&var);//获取这个硬件信息
    //printf("lcd_x = %d, lcd_y = %d, pixel = %d\n", var.xres, var.yres, var.bits_per_pixel);

    //内存映射
    unsigned int *memp = (unsigned int *)mmap(NULL,var.xres*var.yres*var.bits_per_pixel/8, 
                                PROT_WRITE|PROT_READ,MAP_SHARED,fd_lcd,0);
    if(memp == MAP_FAILED)
    {
        perror("mmap");
        return -2;
    }

    //存放准备写入到lcd上的数据
    unsigned int lcd_buf[ bmp_info.width * bmp_info.height];
    memset(lcd_buf, 0, sizeof(lcd_buf));

    //将图片读取到bmp_buf再整合到lcd_buf中
    int w_count,h_count;

	for(h_count = 0; h_count < bmp_info.height; h_count++)
	{
		for(w_count = 0; w_count < bmp_info.width; w_count++)
		{
			//	ret*h_count ，前面多出来的字节数
			lcd_buf[w_count+h_count*bmp_info.width] = bmp_buf[3*w_count+(bmp_info.width*3+ret)*h_count+0] ;
			lcd_buf[w_count+h_count*bmp_info.width] |= bmp_buf[3*w_count+bmp_info.width*3*h_count+ret*h_count+1]<<8;
			lcd_buf[w_count+h_count*bmp_info.width] |= bmp_buf[3*w_count+bmp_info.width*3*h_count+ret*h_count+2]<<16;
		}
	}

    int h = (bmp_info.height + y) > var.yres ? (var.yres - y) : bmp_info.height;
    int w = (bmp_info.width + x) > var.xres ? (var.xres - x) : bmp_info.width;
    //将图片写入到lcd屏幕文件（偏移x，y）
    for(h_count = 0; h_count < h; h_count++)
    {
        for(w_count = 0; w_count < w; w_count++)
        {
            *(memp + (w_count+x) + (h_count+y)*var.xres) = 
                                                                    lcd_buf[w_count + (h - h_count - 1) * w];
        }
    }

    //释放相关资源
    munmap(memp,var.xres*var.yres*var.bits_per_pixel/8);
    fclose(fp_bmp);
    close(fd_lcd);
    return 0;
}

int show_bmp_over(char *path, int x, int y)
{
    int ret = 0;
    struct bitmap_header bmp_header;
    struct bitmap_info bmp_info;
    memset(&bmp_header, 0, 14);
    memset(&bmp_info, 0, 40);

    //打开BMP图片
    FILE *fp_bmp = fopen(path, "r");
    if(fp_bmp == NULL)
    {
        perror("open bmp failed");
        return -1;
    }

    //读取bmp头
    ret = fread(&bmp_header,  14, 1, fp_bmp);
    if(ret != 1)
    {
        perror("read error 14");
        return -1;
    }

    //如果不是bmp格式则返回-1
    if(bmp_header.type != 19778)
    {
        printf("It's not bmp file");
        return -1;
    }

    //读取bmp图片信息，获取图片宽度和高度
    ret = fread(&bmp_info,  40, 1, fp_bmp);
    if(ret != 1)
    {
        perror("read error 40");
    }
    printf("bmp_x = %d, bmp_y =  %d\n", bmp_info.width, bmp_info.height);

    //因为地址对齐，每行需要多补的字节数
	ret = (4-bmp_info.width*3%4)%4; 
    
    //读取图片的缓存空间，大小为图片长*宽+每行对齐多出来的字节数
    unsigned char bmp_buf[bmp_info.height * bmp_info.width * 3 + ret * bmp_info.height];
    memset(bmp_buf, 0, sizeof(bmp_buf));

    //读取bmp图片到bmp_buf
	fread(bmp_buf, sizeof(bmp_buf), 1, fp_bmp);
    
    //打开lcd设备文件
    int fd_lcd = open("/dev/fb0",O_RDWR);
    if(fd_lcd == -1)
    {
        perror("open");
        return -1;
    }
    struct fb_var_screeninfo var;//用于存放屏幕设备的宽度、高度信息
    ioctl(fd_lcd,FBIOGET_VSCREENINFO,&var);//获取这个硬件信息
    printf("lcd_x = %d, lcd_y = %d, pixel = %d\n", var.xres, var.yres, var.bits_per_pixel);

    //内存映射
    unsigned int *memp = (unsigned int *)mmap(NULL,var.xres*var.yres*var.bits_per_pixel/8, 
                                PROT_WRITE|PROT_READ,MAP_SHARED,fd_lcd,0);
    if(memp == MAP_FAILED)
    {
        perror("mmap");
        return -2;
    }

    int count = 0;
    for(count = 0;count < var.xres*var.yres; count++)
    {
        *(memp+count) = 0x00000000;
    }

    //存放准备写入到lcd上的数据
    unsigned int lcd_buf[ bmp_info.width * bmp_info.height];
    memset(lcd_buf, 0, sizeof(lcd_buf));

    //将图片读取到bmp_buf再整合到lcd_buf中
    int w_count,h_count;
	for(h_count = 0; h_count < bmp_info.height; h_count++)
	{
		for(w_count = 0; w_count < bmp_info.width; w_count++)
		{
			//	ret*h_count ，前面多出来的字节数
			lcd_buf[w_count+h_count*bmp_info.width] = bmp_buf[3*w_count+(bmp_info.width*3+ret)*h_count+0] ;
			lcd_buf[w_count+h_count*bmp_info.width] |= bmp_buf[3*w_count+bmp_info.width*3*h_count+ret*h_count+1]<<8;
			lcd_buf[w_count+h_count*bmp_info.width] |= bmp_buf[3*w_count+bmp_info.width*3*h_count+ret*h_count+2]<<16;
		}
	}

    int h = (bmp_info.height + y) > var.yres ? (var.yres - y) : bmp_info.height;
    int w = (bmp_info.width + x) > var.xres ? (var.xres - x) : bmp_info.width;
    //将图片写入到lcd屏幕文件（偏移x，y）
    for(h_count = 0; h_count < h; h_count++)
    {
        for(w_count = 0; w_count < w; w_count++)
        {
            *(memp + (w_count+x) + (h_count+y)*var.xres) = 
                                                                    lcd_buf[w_count + (h - h_count - 1) * w];
        }
    }

    //释放相关资源
    munmap(memp,var.xres*var.yres*var.bits_per_pixel/8);
    fclose(fp_bmp);
    close(fd_lcd);
    return 0;
}
/* 
int layer_merge(unsigned int *img[], int x[], int y[])
{

}
*/
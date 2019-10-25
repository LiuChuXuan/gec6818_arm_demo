#include "v4l2.h"
#include "ui_v4l2.h"

extern "C"
{
#include <stdio.h>
#include "./jpeg/jpeglib.h"
#include "./jpeg/yuyv.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>
#include "lcd.h"
#include <sys/mman.h>
#include <stdlib.h>
}
static unsigned char g_color_buf[FB_SIZE] = {0};

static int lcd_fd;
static int *mmap_fd;
static int get_picture_flag = 0; //设置拍照变量
bool camera_show = false;

//初始化LCD
int lcd_open(void)
{
    lcd_fd = open("/dev/fb0", O_RDWR);

    if(lcd_fd < 0)
    {
        printf("open lcd error\n");
        return -1;
    }

    return 0;
}

int mmap_lcd(void)
{
    mmap_fd = (int *)mmap(NULL,                   //映射区的开始地址，设置为NULL时表示由系统决定映射区的起始地址
                          FB_SIZE,                //映射区的长度
                          PROT_READ | PROT_WRITE, //内容可以被读取和写入
                          MAP_SHARED,             //共享内存
                          lcd_fd,                 //有效的文件描述词
                          0                       //被映射对象内容的起点
                         );
    return lcd_fd;
}

//LCD画点
void lcd_draw_point(unsigned int x, unsigned int y, unsigned int color)
{
    *(mmap_fd + y * 800 + x) = color;
}

//显示摄像头捕捉
int show_video_data(unsigned int x, unsigned int y, unsigned char *pjpg_buf, unsigned int jpg_buf_size)
{
    /*定义解码对象，错误处理对象*/
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    unsigned char *pcolor_buf = g_color_buf;
    unsigned char *pjpg;
    unsigned int i = 0;
    unsigned int color = 0;
    unsigned int x_s = x;
    unsigned int x_e;
    unsigned int y_e;

    pjpg = pjpg_buf;

    /*注册出错处理*/
    cinfo.err = jpeg_std_error(&jerr);

    /*创建解码*/
    jpeg_create_decompress(&cinfo);

    /*直接解码内存数据*/
    jpeg_mem_src(&cinfo, pjpg, jpg_buf_size);

    /*读文件头*/
    jpeg_read_header(&cinfo, TRUE);

    /*开始解码*/
    jpeg_start_decompress(&cinfo);

    x_e = x_s + cinfo.output_width;
    y_e = y + cinfo.output_height;

    /*读解码数据*/
    while(cinfo.output_scanline < cinfo.output_height)
    {
        pcolor_buf = g_color_buf;

        /* 读取jpg一行的rgb值 */
        jpeg_read_scanlines(&cinfo, &pcolor_buf, 1);

        for(i = 0; i < cinfo.output_width; i++)
        {
            /* 获取rgb值 */
            color = *(pcolor_buf + 2);
            color = color | *(pcolor_buf + 1) << 8;
            color = color | *(pcolor_buf) << 16;

            /* 显示像素点 */
            lcd_draw_point(x, y, color);

            pcolor_buf += 3;

            x++;
        }

        /* 换行 */
        y++;
        x = x_s;
    }

    /*解码完成*/
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    return 0;
}

//显示正常jpg图片
int lcd_draw_jpg(unsigned int x, unsigned int y, const char *pjpg_path)
{
    /*定义解码对象，错误处理对象*/
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    unsigned char *pcolor_buf = g_color_buf;
    unsigned char *pjpg;
    unsigned int i = 0;
    unsigned int color = 0;
    unsigned int x_s = x;
    unsigned int x_e;
    unsigned int y_e;
    unsigned int y_n = y;
    unsigned int x_n = x;

    int jpg_fd;
    unsigned int jpg_size;

    if(pjpg_path != NULL)
    {
        /* 申请jpg资源，权限可读可写 */
        jpg_fd = open(pjpg_path, O_RDWR);

        if(jpg_fd == -1)
        {
            printf("open %s error\n", pjpg_path);

            return -1;
        }

        /* 获取jpg文件的大小 */
        jpg_size = file_size_get(pjpg_path);

        if(jpg_size < 3000)
        {
            return -1;
        }

        /* 为jpg文件申请内存空间 */
        pjpg = (unsigned char *)malloc(jpg_size);

        /* 读取jpg文件所有内容到内存 */
        read(jpg_fd, pjpg, jpg_size);
    }
    else
    {
        return -1;
    }

    /*注册出错处理*/
    cinfo.err = jpeg_std_error(&jerr);

    /*创建解码*/
    jpeg_create_decompress(&cinfo);

    /*直接解码内存数据*/
    jpeg_mem_src(&cinfo, pjpg, jpg_size);

    /*读文件头*/
    jpeg_read_header(&cinfo, TRUE);

    /*开始解码*/
    jpeg_start_decompress(&cinfo);

    x_e = x_s + cinfo.output_width;
    y_e = y + cinfo.output_height;

    /*读解码数据*/
    while(cinfo.output_scanline < cinfo.output_height)
    {
        pcolor_buf = g_color_buf;

        /* 读取jpg一行的rgb值 */
        jpeg_read_scanlines(&cinfo, &pcolor_buf, 1);

        for(i = 0; i < cinfo.output_width; i++)
        {
            /* 不显示的部分 */
            /* if(y_n>g_jpg_in_jpg_y && y_n<g_jpg_in_jpg_y+240)
                if(x_n>g_jpg_in_jpg_x && x_n<g_jpg_in_jpg_x+320)
                {
                    pcolor_buf +=3;
                    x_n++;
                    continue;
                } */

            /* 获取rgb值 */
            color = *(pcolor_buf + 2);
            color = color | *(pcolor_buf + 1) << 8;
            color = color | *(pcolor_buf) << 16;

            /* 显示像素点 */
            lcd_draw_point(x_n, y_n, color);

            pcolor_buf += 3;

            x_n++;
        }

        /* 换行 */
        y_n++;

        x_n = x_s;
    }

    /*解码完成*/
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);

    if(pjpg_path != NULL)
    {
        /* 关闭jpg文件 */
        close(jpg_fd);

        /* 释放jpg文件内存空间 */
        free(pjpg);
    }

    return 0;
}

//LCD关闭
void lcd_close(void)
{

    /* 取消内存映射 */
    munmap(mmap_fd, FB_SIZE);

    /* 关闭LCD设备 */
    close(lcd_fd);
}

//获取jpg文件的大小
unsigned long file_size_get(const char *pfile_path)
{
    unsigned long filesize = -1;
    struct stat statbuff;

    if(stat(pfile_path, &statbuff) < 0)
    {
        return filesize;
    }
    else
    {
        filesize = statbuff.st_size;
    }

    return filesize;
}


//实时监控函数
void *real_time_yuyv(void *arg)
{
    //定义结构体变量
    struct jpg_data video_buf;
    lcd_open();
    mmap_lcd();
    //初始化摄像头
    linux_v4l2_yuyv_init("/dev/video7");
    //开启摄像头捕捉
    linux_v4l2_start_yuyv_capturing();

    int fd;
    int jpg_num = 0;
    char pic_name[20];

    while(1)
    {
        //获取摄像头捕捉的画面
        linux_v4l2_get_yuyv_data(&video_buf);

        //显示摄像头捕捉的画面
        if(true == camera_show)
        {
            show_video_data(0, 0, video_buf.jpg_data, video_buf.jpg_size);
        }

        if(get_picture_flag == 1)  //如果点击抓拍，保存图片信息
        {
            //创建一个新的文件
            bzero(pic_name, 20);

            sprintf(pic_name, "/video_jpg/%d.jpg", jpg_num);

            fd = open(pic_name, O_RDWR | O_CREAT, 0777);

            if(-1 == fd)
            {
                perror("create jpg failed");
                continue;
            }

            write(fd, video_buf.jpg_data, video_buf.jpg_size);

            close(fd);

            get_picture_flag = 0;
            jpg_num++;
        }
    }

    //摄像头解除初始化
    linux_v4l2_yuyv_quit();

    lcd_close();
}


V4L2::V4L2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::V4L2)
{
    ui->setupUi(this);

    //创建一条线程实时监控
    pthread_t pid;
    pthread_create(&pid, NULL, real_time_yuyv, NULL);
}

V4L2::~V4L2()
{
    delete ui;
}

void V4L2::on_pushButton_clicked()
{
    camera_show = true;
}

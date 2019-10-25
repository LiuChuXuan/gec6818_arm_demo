#ifndef __LCD_H__
#define __LCD_H__

#define LCD_WIDTH  			800
#define LCD_HEIGHT 			480
#define FB_SIZE				(LCD_WIDTH * LCD_HEIGHT * 4)



typedef struct VideoBuffer {
    void   *start;  //映射到用户空间的地址
    size_t  length; //采集到数据的长度
} VideoBuffer1; //存放采集数据的位置

#define BUFFER_COUNT 4
VideoBuffer1 framebuf[BUFFER_COUNT]; 


//函数声明
unsigned long file_size_get(const char *pfile_path);
void lcd_close(void);
int show_video_data(unsigned int x,unsigned int y,unsigned char *pjpg_buf,unsigned int jpg_buf_size);
int lcd_draw_jpg(unsigned int x,unsigned int y,const char *pjpg_path);
void lcd_draw_point(unsigned int x,unsigned int y, unsigned int color);
int mmap_lcd(void);
int lcd_open(void);
#endif

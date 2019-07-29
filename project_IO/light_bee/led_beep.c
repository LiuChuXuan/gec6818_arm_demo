#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "input_sc.h"
#include "show_bmp.h"
#define TEST_MAGIC 'x'                    //定义幻数

#define LED1 _IO(TEST_MAGIC,0)
#define LED2 _IO(TEST_MAGIC,1)
#define LED3 _IO(TEST_MAGIC,2)
#define LED4 _IO(TEST_MAGIC,3)

int led_beep(void)
{
    int status_beep = 1;
    int status_led[4];
    memset(&status_led, 1, sizeof(status_led));
    int x = 0, y = 0;

    //访问蜂鸣器的节点文件
    int fd_beep = open("/dev/beep",O_RDWR);
	if(fd_beep < 0)
	{
		printf("open beep error!\n");
	}

    ioctl(fd_beep, status_beep, 1); //不响



	//访问LED的节点文件
	int fd_led = open("/dev/Led",O_RDWR);
	if(fd_led < 0)
    {
		printf("pls insmod driver!\n");
    }

    //使用ioctl函数关闭所有灯
    ioctl(fd_led, LED1, status_led[0]);
    ioctl(fd_led, LED2, status_led[1]);
    ioctl(fd_led, LED3, status_led[2]);
    ioctl(fd_led, LED4, status_led[3]);

    show_bmp("./ui_img/menu_led_bee.bmp", 0, 0);    
    show_bmp("./ui_img/led_close.bmp", 50, 90);
    show_bmp("./ui_img/led_close.bmp", 250, 90);
    show_bmp("./ui_img/led_close.bmp", 450, 90);
    show_bmp("./ui_img/led_close.bmp", 650, 90);




    while(1)
    {
        get_xy_release(&x, &y);
        if(y < 280 && x < 200)
        {
            status_led[0] = !status_led[0];
            ioctl(fd_led, LED1, status_led[0]);
            if(status_led[0])
            {
                show_bmp("./ui_img/led_close.bmp", 50, 90);
            }
            else
            {
                show_bmp("./ui_img/led_open.bmp", 50, 90);
            }
        }
        else if(y < 280 && x > 200 && x < 400)
        {
            status_led[1] = !status_led[1];
            ioctl(fd_led, LED2, status_led[1]);
            if(status_led[1])
            {
                show_bmp("./ui_img/led_close.bmp", 250, 90);
            }
            else
            {
                show_bmp("./ui_img/led_open.bmp", 250, 90);
            }
        }
        else if(y < 280 && x > 400 && x < 600)
        {
            status_led[2] = !status_led[2];
            ioctl(fd_led, LED3, status_led[2]);
            if(status_led[2])
            {
                show_bmp("./ui_img/led_close.bmp", 450, 90);
            }
            else
            {
                show_bmp("./ui_img/led_open.bmp", 450, 90);
            }
        }
        else if(y < 280 && x > 600 && x < 800)
        {
            status_led[3] = !status_led[3];
            ioctl(fd_led, LED4, status_led[3]);
            if(status_led[3])
            {
                show_bmp("./ui_img/led_close.bmp", 650, 90); 
            }
            else
            {
                show_bmp("./ui_img/led_open.bmp", 650, 90); 
            }
        }
        else if(y > 280 && x < 400)
        {
            status_beep = !status_beep;
            ioctl(fd_beep, status_beep, 1);
        }
        else if(y > 280 && x > 400)
        {
            break;
        }
    }
	
	//3.回收资源
	close(fd_led);
    close(fd_beep);
	return 0;
}
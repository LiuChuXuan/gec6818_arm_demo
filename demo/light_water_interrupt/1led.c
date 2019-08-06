/**
 *  练习：流水灯，点击某个按钮后中途打断流水灯（线程）
 */

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define TEST_MAGIC 'x'                    //定义幻数

#define LED1 _IO(TEST_MAGIC,0)
#define LED2 _IO(TEST_MAGIC,1)
#define LED3 _IO(TEST_MAGIC,2)
#define LED4 _IO(TEST_MAGIC,3)

static inline void close_all_led(int fd_led);
static inline void open_all_led(int fd_led);

void* get_xy(void *flag);

int main(void)
{
    int flag = 0;       //用于标记是否有触摸屏按下事件发生
    pthread_t tid;      //定义线程id号
    pthread_attr_t attr;//定义线程属性
    //初始化线程属性变量
    pthread_attr_init(&attr);

    //把线程属性设置到化线程属性变量
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int count = 0;      //用于流水灯循环时计数
    int status_led[4];  //每盏灯的状态，1为灭，0为亮
    memset(&status_led, 1, sizeof(status_led));

    //创建子线程
    pthread_create(&tid, &attr, get_xy, (void *)(&flag));

	//访问LED的节点文件
	int fd_led = open("/dev/Led",O_RDWR);
	if(fd_led < 0)
    {
		printf("pls insmod driver!\n");
    }

    close_all_led(fd_led);

    //流水灯
    status_led[0] = 0;//先点亮第一盏灯
    ioctl(fd_led, LED1, status_led[0]);
    sleep(1);
    while(flag != 1)
    {
        status_led[count] = !status_led[count];
        ioctl(fd_led, _IO(TEST_MAGIC,count), status_led[count]);
        count = (count + 1) % 4;
        status_led[count] = !status_led[count];
        ioctl(fd_led, _IO(TEST_MAGIC,count), status_led[count]);
        sleep(1);
    }
    
    //接收到触摸屏信号后闪两下然后退出程序
    open_all_led(fd_led);
    sleep(1);
    close_all_led(fd_led);
    sleep(1);
    open_all_led(fd_led);
    sleep(1);
    close_all_led(fd_led);
    sleep(1);

    //销毁线程属性变量
    pthread_attr_destroy(&attr);
    return 0;
}

//使用ioctl函数关闭所有灯
static inline void close_all_led(int fd_led)
{
    ioctl(fd_led, LED1, 1);
    ioctl(fd_led, LED2, 1);
    ioctl(fd_led, LED3, 1);
    ioctl(fd_led, LED4, 1);
}
//使用ioctl函数打开所有灯
static inline void open_all_led(int fd_led)
{
    ioctl(fd_led, LED1, 0);
    ioctl(fd_led, LED2, 0);
    ioctl(fd_led, LED3, 0);
    ioctl(fd_led, LED4, 0);
}


//获取按下时的x0,y0和松手时的x1,y1
void* get_xy(void *flag)
{
    struct input_event evt;
	memset(&evt, 0, sizeof(evt));

    int fd = open("/dev/input/event0",O_RDWR);
    if(fd == -1)
	{
        perror("open event0 failed");
        return (void *)-1;
    }

	while(read(fd,&evt,sizeof(evt))>0)
	{
        if(evt.value == 1)
        {
            *((int *)flag) = 1;
        }
    }
    return NULL;
}
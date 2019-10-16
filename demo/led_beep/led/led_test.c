#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#define TEST_MAGIC 'x' //定义幻数

#define LED1 _IO(TEST_MAGIC, 0)
#define LED2 _IO(TEST_MAGIC, 1)
#define LED3 _IO(TEST_MAGIC, 2)
#define LED4 _IO(TEST_MAGIC, 3)

int main()
{
	//1.访问LED的节点文件
	int fd = open("/dev/Led", O_RDWR);
	if (fd < 0)
		printf("pls insmod driver!\n");

	//2.使用ioctl函数控制灯
	while (1)
	{
		ioctl(fd, LED1, 0);
		sleep(1);
		ioctl(fd, LED1, 1);
		sleep(1);
	}

	//3.回收资源
	close(fd);

	return 0;
}
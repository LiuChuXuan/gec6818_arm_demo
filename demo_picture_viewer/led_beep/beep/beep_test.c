#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

int main()
{
	//1.访问蜂鸣器的节点文件
	int fd = open("/dev/beep",O_RDWR);
	if(fd < 0)
	{
		printf("open beep error!\n");
	}
	
	//2.使用ioctl函数控制蜂鸣器状态
	while(1)
	{
		ioctl(fd,0,1); //响
		sleep(1);
		ioctl(fd,1,1); //不响
		sleep(1);
	}
	
	//3.回收资源
	close(fd);

	return 0;
}
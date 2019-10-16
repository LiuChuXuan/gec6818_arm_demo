#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>

typedef struct
{
	int mode;
	char basepath[256];
	char path[256];
	char buf[4096];
} buf, *buf_t;

int basename(char *name, const char *path);
int scan_dir(int soc_fd, buf_t data);

int main(int argc, const char **argv)
{
	//1、买手机-->创建套接字--》获取待连接套接字
	int soc_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (soc_fd == -1)
	{
		perror("socket failed!");
		return -1;
	}

	//定义ipv4地址结构体变量
	struct sockaddr_in ser_addr;
	bzero(&ser_addr, sizeof(ser_addr));

	//配置服务器端IPV4地址结构体
	ser_addr.sin_family = AF_INET;							//选择ipv4地址族
	ser_addr.sin_port = htons(40000);						//端口号：0~65535-->把主机字节序转化为网络字节序
	ser_addr.sin_addr.s_addr = inet_addr("192.168.21.191"); //IP地址-->把主机字节序转化为网络字节序

	//打电话
	int ret = connect(soc_fd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
	if (ret == -1)
	{
		perror("connect failed!\n");
		close(soc_fd);
		return -1;
	}

	buf_t data = (buf_t)calloc(1, sizeof(buf));
	if (data == NULL)
	{
		perror("data calloc failed");
		close(soc_fd);
		return -1;
	}

	if (strlen("../../thread_pool_copy") < 256)
	{
		strcpy(data->path, "../../thread_pool_copy");
		strcpy(data->basepath, "../../");
	}
	else
	{
		perror("filepath length >= 256");
		free(data);
		return -1;
	}

	scan_dir(soc_fd, data);

	//6、挂电话
	close(soc_fd);
	return 0;
}

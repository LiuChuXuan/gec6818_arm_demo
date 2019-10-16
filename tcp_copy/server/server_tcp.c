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

int server_recv(int acc_fd, buf_t data);

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

	//配置IPV4地址结构体
	ser_addr.sin_family = AF_INET;							//选择ipv4地址族
	ser_addr.sin_port = htons(40000);						//端口号：0~65535-->把主机字节序转化为网络字节序
	ser_addr.sin_addr.s_addr = inet_addr("192.168.21.191"); //IP地址-->把主机字节序转化为网络字节序

	//2、绑定号码（IP地址和端口号）
	int ret = bind(soc_fd, (struct sockaddr *)&ser_addr, sizeof(ser_addr));
	if (ret == -1)
	{
		perror("bind faield!\n");
		close(soc_fd);
		return -1;
	}

	//3、设置铃声(监听套接字)-->把待连接套接字变成监听套接字
	ret = listen(soc_fd, 1);
	if (ret == -1)
	{
		perror("listen faield!\n");
		close(soc_fd);
		return -1;
	}
	//定义ipv4地址结构体变量
	struct sockaddr_in client_addr;

	int len = sizeof(struct sockaddr_in);
	bzero(&client_addr, sizeof(client_addr));

	//4、阻塞等电话-->等待连接-->连接上后可以获得已连接的套接字--》
	//每连接上一个新的客户端，就会产生一个新的套接字
	int acc_fd = accept(soc_fd,
						(struct sockaddr *)&client_addr, &len);

	if (acc_fd == -1)
	{
		perror("accept failed!");
		close(soc_fd);
		return -1;
	}

	//unsigned short -->%hu
	//网络字节序二进制ip地址转化为字符串类型的ip地址
	printf("%s: %hu\n", //把网络字节序的端口号转化为主机字节序的端口号
		   inet_ntoa(client_addr.sin_addr),
		   ntohs(client_addr.sin_port));

	buf_t data = (buf_t)calloc(1, sizeof(buf));
	if (data == NULL)
	{
		perror("data calloc failed");
		close(acc_fd);
		close(soc_fd);
		return -1;
	}

	server_recv(acc_fd, data);

	//6、挂电话
	close(acc_fd);
	close(soc_fd);
	return 0;
}

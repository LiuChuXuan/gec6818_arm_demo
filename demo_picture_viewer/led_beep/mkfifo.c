#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	//判断文件是否存在
	if(access("/home/gec/myfifo", F_OK))
	{
		//创建一个有名管道文件
		if(mkfifo("/home/gec/myfifo", 0777))
		{
			
			perror("mkfifo failed!");
			
		}
		else
		{
			printf("mkfifo success!\n");
		}
				
	}
	else
	{
		printf("fifo exit!\n");
		
	}
	
	
	return 0;
}
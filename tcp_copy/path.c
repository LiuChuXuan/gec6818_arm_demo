#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int basename(char *name,const char *path)
{
	const int size = strlen(path);
	int count = size - 2;
	int distance = 0;

	while(count >= 0)
	{
		if(*(path + count) == '/')
		{
			break;
		}
		count -= 1;
	}
	if(count == -1)
	{
		strcpy(name,path);
		return 0;
	}

	while(count < size)
	{
		count += 1;
		*(name + distance) = *(path + count);
		distance += 1;
	}
	return 0;
}

int pathcat(char *dest, const char *arg1, const char *arg2)
{
	int arg1_len = strlen(arg1);
	int arg2_len = strlen(arg2);

	//判断目录字符串最后一个字符是不是'/'
	if(*(arg1 + arg1_len - 1) != '/')     
	{
		if(arg1_len + arg2_len + 1 < 256)
		{
			strcat(dest,arg1);
			strcat(dest,"/");//如果不是'/'则要加上
			strcat(dest,arg2);
		}
		else
		{
			perror("path length too long");
			return -1;
		}
	}
	else//如果有'/'就不用加上
	{
		if(arg1_len + arg2_len < 256)
		{
			strcat(dest, arg1);
			strcat(dest, arg2);
		}
		else
		{
			perror("path length too long");
			return -1;
		}
	}
	return 0;
}
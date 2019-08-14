#include "main.h"


int basename(char **name,char *path)
{
	/* 012345 */
	/* main.c */
	/* 123456 */
	const int size = strlen(path);//=6
	int count = size;		//=6
	int distance = 0;		//=0

	//			n,i,a != '/'           4,3-2>0 
	while((*(path + count) != '/') && (count - 2 > 0))
	{
		count -= 1;
	}

	while(count > 0)
	{
		if(*(path + count - 2) == '/')
		{
			*name = (char *)calloc(1, size - count - 2);
			if(*name == NULL)
			{
				perror("basename calloc failed");
				return -1;
			}
			break;
		}
		count -= 1;
	}
	if(count == 0)
	{
		*name = (char *)calloc(1, size + 1);
		if(*name == NULL)
		{
			perror("basename calloc failed");
			return -1;
		}
		strcpy(*name,path);
		return 0;
	}

	int i = 0;
	while(count - 2 < size)
	{
		count += 1;
		*(*name + distance) = *(path + count - 2);
		distance += 1;
	}
	return 0;
}

int pathcat(char **dest, const char *arg1, const char *arg2)
{
	int size = strlen(arg1);
	//判断目录字符串最后一个字符是不是'/'
	if(*(arg1 + size - 1) != '/')     
	{
		//如果不是'/'，那要加上2，一个是'/'，一个是'/0'
		size += strlen(arg2) + 2;    
		*dest = (char *)calloc(1, size);
		if(*dest != NULL)
		{
			strcat(*dest,arg1);
			strcat(*dest,"/");
			strcat(*dest,arg2);
		}
		else
		{
			perror("file path calloc failed");
			return -1;
		}
	}
	else//如果是'/'则只需要加上1，即'/0'所占的空间
	{
		size += strlen(arg2) + 1;
		*dest = (char *)calloc(1, size);
		if(*dest != NULL)
		{
			strcat(*dest, arg1);
			strcat(*dest, arg2);
		}
		else
		{
			perror("file path calloc failed");
			return -1;
		}
	}
	return 0;
}
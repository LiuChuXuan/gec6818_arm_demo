#include "main.h"

//任务函数，要执行的任务写到里面:复制文件
void *do_task(void *arg)
{
	int ret = 0;
	int index = 0;
	char *src = ((char **)arg)[0];
	char *dest = NULL;
	int dest_len = strlen(((char **)arg)[1]);
	if (*(((char **)arg)[1] + dest_len - 1) == '/')
	{
		dest = (char *)calloc(1, dest_len);
		if (dest == NULL)
		{
			perror("do_task dest calloc failed");
		}
		strncat(dest, ((char **)arg)[1], dest_len - 1);
		free(((char **)arg)[1]);
	}
	else
	{
		dest = ((char **)arg)[1];
	}

	while (1)
	{
		while (1)
		{
			if (*(dest + index) == '/')
			{
				break;
			}
			if (*(dest + index) == '\0')
			{
				break;
			}
			index += 1;
		}

		if (*(dest + index) == '\0')
		{
			printf("break index = %d\n", index);
			break;
		}

		char temp[index + 2];
		memset(temp, 0, index + 2);

		strncat(temp, dest, index + 1);

		ret = access(temp, F_OK);
		if (ret != 0)
		{
			mkdir(temp, 0777);
		}

		index += 1;
	}
	FILE *fp_src = fopen(src, "r");
	if (fp_src == NULL)
	{
		perror("src open failed");
	}
	FILE *fp_dest = fopen(dest, "w+");
	if (fp_dest == NULL)
	{
		printf("dest:%s\n", dest);
		perror("dest open failed");
	}

	char buffer[1];

	while ((ret = fread(buffer, 1, 1, fp_src)) == 1)
	{
		if (fwrite(buffer, 1, 1, fp_dest) < 1)
		{
			perror("fwrite failed!");
		}
	}
	if (ret < 1)
	{
		if (feof(fp_src))
		{
			printf("copy success \n");
		}
	}

	free(src);
	free(dest);
	//free(arg);
	fclose(fp_dest);
	fclose(fp_src);
	printf("free success\n");
	return NULL;
}

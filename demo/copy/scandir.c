#include "main.h"

//不断地读取目录里面的文件，插入到do_task中
//第一个参数src = argv[1], 第二个参数dest = argv[2]
int scan_dir(char *src, const char *dest)
{
	//申请堆空间内存，用于传到do_task中，将在那边释放
	char *path[2] = {NULL, NULL};
    
    /**不知道为什么不可以这样     
        char **path = calloc(2,sizeof(char *));
        if(path == NULL)
        {
            perror("calloc failed.");
            return -1;
        }
    */

	//用于存放文件信息
    struct stat file_info;
    memset(&file_info, 0, sizeof(file_info));
    struct dirent *p = NULL;
	int ret = 0;
	char *dirname = NULL;
	basename(&dirname,src);

	//获取文件信息
	lstat(src, &file_info);

	//如果不是目录就加入复制任务的队列中
	if((file_info.st_mode & S_IFMT) != S_IFDIR)
	{
		path[0] = (char *)calloc(1, strlen(src) + 1);
		strcpy(path[0], src);


		ret = pathcat(&(path[1]), dest, dirname);
		if(ret == -1)
		{
			perror("pathcat failed");
		}

		//add_task(pool, do_task, (void *)path);
        do_task((void *)path);

		return 0;
	}

	//否则如果是目录则打开目录
	DIR *dp = opendir(src);
	if(dp == NULL)
	{
		perror("opendir failed");
		return -1;
	}

	while((p = readdir(dp)) != NULL)
    {
        //如果是"."和".."则跳过
        if((strcmp(p->d_name, ".") == 0) || \
		  (strcmp(p->d_name, "..") == 0))
        {
            continue;
        }

        ret = pathcat(&(path[0]), src, p->d_name);
        if(ret == -1)
        {
            perror("pathcat failed");
        }

		memset(&file_info, 0, sizeof(file_info));
        lstat(path[0], &file_info);
		//如果不是目录则将文件插入do_task中
		if((file_info.st_mode & S_IFMT) != S_IFDIR)
		{
			char *temp = NULL;
			ret = pathcat(&temp, dest, dirname);
			if(ret == -1)
			{
				perror("pathcat failed");
			}
			ret = pathcat(&(path[1]), temp, p->d_name);
			if(ret == -1)
			{
				perror("pathcat failed");
			}
			free(temp);
			temp = NULL;
			printf("dirent add task!!\n");
            do_task((void *)path);
			//add_task(pool, do_task, (void *)path);
		}
		else//否则是目录就递归执行
		{
			ret = pathcat(&(path[1]), dest, dirname);
			if(ret == -1)
			{
				perror("pathcat failed");
			}

			scan_dir(path[0], path[1]);
			free(path[0]);
			free(path[1]);
            //free(path);
		}
	}
	closedir(dp);
	return 0;
}

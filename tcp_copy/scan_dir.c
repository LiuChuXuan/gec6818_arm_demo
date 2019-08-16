#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct{
    int mode;
    char basepath[256];
    char path[256];
    char buf[4096];
}buf, *buf_t;

int client_send(int soc_fd,buf_t data);
int pathcat(char *dest, const char *arg1, const char *arg2);

int scan_dir(int soc_fd, buf_t data)
{
    //用于存放文件信息
    struct stat file_info;
    memset(&file_info, 0, sizeof(file_info));
    struct dirent *p = NULL;
	int ret = 0;
    char path[256];
    char temp[256];

	//获取文件信息
	lstat(data->path, &file_info);

    //如果不是目录就发送
	if((file_info.st_mode & S_IFMT) != S_IFDIR)
	{
        client_send(soc_fd,data);
        free(data);
		return 0;
	}

	//否则如果是目录则打开目录
	DIR *dp = opendir(data->path);
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

        memset(path, 0, 256);
        ret = pathcat(path, data->path, p->d_name);

        if(ret == -1)
        {
            perror("pathcat failed");
        }

		memset(&file_info, 0, sizeof(file_info));
        lstat(path, &file_info);

        // temp  <-- data->path
        memset(temp, 0, 256);
        strcpy(temp, data->path);

        // data->path <-- path
        memset(data->path, 0, 256);
        strcpy(data->path, path);

		//如果不是目录则发送文件
		if((file_info.st_mode & S_IFMT) != S_IFDIR)
		{
            client_send(soc_fd, data);
        }
        else//否则是目录则递归
        {
            scan_dir(soc_fd, data);
        }

        // data->path <-- temp
        memset(data->path, 0, 256);
        strcpy(data->path, temp);
	}
	closedir(dp);
    free(data);
	return 0;
}
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
typedef struct{
    int mode;
    char basepath[256];
    char path[256];
    char buf[4096];
}buf, *buf_t;

int pathcat(char *dest, const char *arg1, const char *arg2);

//basepath = ../../aa/
//    path = ../../aa/path/file
//           0123456789012345678
int savedata(buf_t data, char *basepath)
{
    int ret = 0;
    int ret_wr = 0;
    int index = 0;
    int size = sizeof(buf);
    int dest_len = strlen(basepath);
    int src_len = strlen(data->basepath);

    char dest[256];
    memset(dest, 0, 256);

    pathcat(dest, basepath, (data->path) + src_len);

    dest_len = strlen(dest);
    if(dest[dest_len - 1] == '/')
    {
        dest[dest_len - 1] = '\0';
    }

	while(1)
	{
		while(1)
		{
			if(*(dest + index) == '/')
			{
				break;
			}
			if(*(dest + index) == '\0')
			{
				break;
			}
			index += 1;
		}

		if(*(dest + index) == '\0')
		{
			break;
		}

		char temp[index + 2];
		memset(temp, 0, index + 2);

		strncat(temp, dest, index + 1);

		ret = access(temp, F_OK);
		if(ret != 0)
		{
			mkdir(temp, 0777);
		}

		index += 1;
	}
    FILE *fp = NULL;
    if(data->mode == 0)
    {
        fp = fopen(dest,"w");
        if(fp == NULL)
        {
            perror("open failed");
            return -1;
        }
    }
    else if(data->mode == 1)
    {
        fp = fopen(dest,"a");
        if(fp == NULL)
        {
            perror("open failed");
            return -1;
        }
    }
    
    index = 0;
    while(size > 0)
    {
        ret_wr = fwrite((data->buf) + index, \
                        size, 1, fp);
        if(ferror(fp))
        {
            perror("fread error");
            clearerr(fp);
            break;
        }

        size -= ret_wr;
        index += ret_wr;

        if(feof(fp))
        {
            clearerr(fp);
            break;
        }
    }

    fclose(fp);
    return 0;
}


int server_recv(int acc_fd, buf_t data)
{
    int ret_rd = 0,   \
        index  = 0,   \
        size   = sizeof(buf);

    while(1)
    {
        index = 0;
        size = sizeof(buf);
        memset(data, 0, sizeof(buf));
	    while(1)
	    {
            ret_rd = read(acc_fd,       \
                          data + index, \
                          size);
            if(ret_rd == -1)
            {
                perror("read acc_fd failed");
                return -1;
            }
            size -= ret_rd;
            index += ret_rd;
            if(size == 0)
            {
                savedata(data, ".");
                break;
            }
	    }
        if(data->mode == 0 || data->mode == 1)
        {
            savedata(data, ".");
        }
        else if(data->mode == 2)
        {
            break;
        }
        else
        {
            perror("data mode error");
            return -1;
        }
    }
    return 0;
}
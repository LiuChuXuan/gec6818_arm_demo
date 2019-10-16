#include <stdio.h>
#include <unistd.h>
#include <string.h>
typedef struct
{
    int mode;
    char basepath[256];
    char path[256];
    char buf[4096];
} buf, *buf_t;

int client_send(int soc_fd, buf_t data)
{
    FILE *fp = fopen(data->path, "r");
    if (fp == NULL)
    {
        perror("open failed");
        return -1;
    }

    printf("----------\n");
    printf("mode:%d\n", data->mode);
    printf("data->path:%s\n", data->path);
    printf("basepath:%s\n", data->basepath);
    printf("----------\n");

    int ret_rd = 0,
        ret_wr = 0,
        size = sizeof(buf),
        index = 0;

    while (1)
    {
        memset(data->buf, 0, 4096);

        ret_rd = fread(data->buf, 4096, 1, fp);
        if (ferror(fp))
        {
            perror("fread error");
            clearerr(fp);
            break;
        }

        index = 0;
        size = sizeof(buf);
        while (size > 0)
        {
            ret_wr = write(soc_fd,
                           data + index,
                           size);

            size -= ret_wr;
            index += ret_wr;
            if (data->mode == 0)
            {
                data->mode = 1;
            }
        }
        if (feof(fp))
        {
            clearerr(fp);
            break;
        }
    }
    printf("send complete!\n");
    return 0;
}

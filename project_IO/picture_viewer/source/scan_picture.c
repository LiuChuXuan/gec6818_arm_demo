#include "scan_picture.h"

int scan_picture(char *dir_path, node_t head)
{
    struct stat file_info;
    memset(&file_info, 0, sizeof(file_info));
    struct dirent *p = NULL;

    //通过传进来的path创建 目录指针
    DIR *dp = opendir(dir_path);
    if (dp == NULL)
    {
        perror("opendir failed");
        return -1;
    }

    //扫描目录下的所有文件
    while ((p = readdir(dp)) != NULL)
    {
        //2.如果是"."和".."则跳过
        if ((strcmp(p->d_name, ".") == 0) || (strcmp(p->d_name, "..") == 0))
        {
            continue;
        }
        char *file_path = NULL;
        int str_size = 0;

        str_size = strlen(dir_path);
        if (*(dir_path + str_size - 1) != '/') //判断目录字符串最后一个字符是不是'/'
        {
            str_size += strlen(p->d_name) + 2; //如果不是'/'，那要加上2，一个是'/'，一个是'/0'
            file_path = (char *)malloc(str_size);
            if (file_path != NULL)
            {
                memset(file_path, 0, str_size);
                strcat(file_path, dir_path);
                strcat(file_path, "/");
                strcat(file_path, p->d_name);
            }
            else
            {
                perror("file path malloc failed");
                return -1;
            }
        }
        else //如果是'/'则只需要加上1，即'/0'所占的空间
        {
            str_size += strlen(p->d_name) + 1;
            file_path = (char *)malloc(str_size);
            if (file_path != NULL)
            {
                memset(file_path, 0, str_size);
                strcat(file_path, dir_path);
                strcat(file_path, p->d_name);
            }
            else
            {
                perror("file path malloc failed");
                return -1;
            }
        }

        lstat(file_path, &file_info);

        if (((file_info.st_mode & S_IFMT) == S_IFBLK) || ((file_info.st_mode & S_IFMT) == S_IFCHR) || ((file_info.st_mode & S_IFMT) == S_IFIFO) || ((file_info.st_mode & S_IFMT) == S_IFLNK) || ((file_info.st_mode & S_IFMT) == S_IFSOCK))
        {
            continue;
        }

        //如果是bmp图片，则将其路径放进链表里
        if ((file_info.st_mode & S_IFMT) == S_IFREG)
        {
            int ret = is_bmp(file_path);
            if (ret == 1)
            {
                list_insert_node(head, file_path, str_size, -1);
            }
            else
            {
                free(file_path);
                continue;
            }
        }

        //如果是文件目录，则递归执行
        else if ((file_info.st_mode & S_IFMT) == S_IFDIR)
        {
            scan_picture(file_path, head);
            free(file_path);
        }
    }
    closedir(dp);
    return 0;
}

//输入文件路径，判断该文件是不是bmp图片
//如果是，返回1；如果不是，返回0；出错返回-1
int is_bmp(char *path)
{
    int ret = 0;
    struct bitmap_header bmp_header;
    memset(&bmp_header, 0, 14);

    FILE *fp_bmp = fopen(path, "r");
    if (fp_bmp == NULL)
    {
        //perror("is_bmp open failed");
        fclose(fp_bmp);
        return -1;
    }

    //读取bmp头
    ret = fread(&bmp_header, 14, 1, fp_bmp);
    if (ret != 1)
    {
        //printf("bmp read ret = %d\n",ret);
        //perror("is_bmp read error 14");
        fclose(fp_bmp);
        return -2;
    }

    //如果是bmp格式则返回1
    if (bmp_header.type == 19778)
    {
        fclose(fp_bmp);
        return 1;
    }
    //如果不是bmp格式则返回0
    else
    {
        fclose(fp_bmp);
        return 0;
    }
}

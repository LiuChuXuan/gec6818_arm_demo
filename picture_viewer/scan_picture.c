#include "scan_picture.h"

//输入文件路径，判断该文件是不是bmp图片
//如果是，返回1；如果不是，返回0；出错返回-1
int is_bmp(char *path)
{
    int ret = 0;
    struct bitmap_header bmp_header;
    memset(&bmp_header, 0, 14);

    FILE *fp_bmp = fopen(path, "r");
    if(fp_bmp == NULL)
    {
        //perror("is_bmp open failed");
        fclose(fp_bmp);
        return -1;
    }

    //读取bmp头
    ret = fread(&bmp_header,  14, 1, fp_bmp);
    if(ret != 1)
    {
        //printf("bmp read ret = %d\n",ret);
        //perror("is_bmp read error 14");
        fclose(fp_bmp);
        return -2;
    }

    //如果是bmp格式则返回1
    if(bmp_header.type == 19778)
    {
        printf("It's not bmp file");
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

int scan_picture(char *path, node_t head)
{
    printf("path = %s\n",path);
    struct stat file_info;
    memset(&file_info, 0, sizeof(file_info));
    struct dirent *p = NULL;

    DIR *dp = opendir(path);
    if(dp == NULL)
    {
        perror("opendir failed");
        return -1;
    }

    //扫描目录下的所有文件
    while((p = readdir(dp)) != NULL)
    {
        //2.如果是"."和".."则跳过
        if((strcmp(p->d_name, ".") == 0) || (strcmp(p->d_name, "..") == 0))
        {
            continue;
        }
        
        stat(p->d_name, &file_info);

        if(((file_info.st_mode & S_IFMT) == S_IFBLK)
        ||((file_info.st_mode & S_IFMT) == S_IFCHR)
        ||((file_info.st_mode & S_IFMT) == S_IFIFO)
        ||((file_info.st_mode & S_IFMT) == S_IFLNK)
        ||((file_info.st_mode & S_IFMT) == S_IFSOCK))
        {
            continue;
        }

        //把打开的文件路径放到str_temp字符串中（堆空间）
        char *path_tmp = NULL;
        int str_length = 0;

        str_length = strlen(path);
        if(*(path + str_length - 1) != '/')
        {
            str_length += strlen(p->d_name) + 2;
            path_tmp = (char*)malloc(str_length);
            if(path_tmp != NULL)
            {
                memset(path_tmp, 0, str_length);
                strcat(path_tmp,path);
                strcat(path_tmp,"/");
                strcat(path_tmp,p->d_name);
            }
        }
        else
        {
            str_length += strlen(p->d_name) + 1;
            path_tmp = (char*)malloc(str_length);
            if(path_tmp != NULL)
            {
                memset(path_tmp, 0, str_length);
                strcat(path_tmp,path);
                strcat(path_tmp,p->d_name);
            }
        }
        printf("%s\n",path_tmp);

        //如果是bmp图片，则将其路径放进链表里
        if((file_info.st_mode & S_IFMT) == S_IFREG)
        {
            int ret = is_bmp(path_tmp);
            if(ret == 1)
            {
                node_insert(head, path_tmp, -1);
            }
            else
            {
                free(path_tmp);
                continue;
            }
        }

        //如果是文件目录，则递归执行
        else if((file_info.st_mode & S_IFMT) == S_IFDIR)
        {
            perror("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
            printf("11111");
            scan_picture(path_tmp, head);
            free(path_tmp);
        }
    }
    closedir(dp);
    return 0;
}

//打印出当前目录的文件信息
int ls_dir(char *path)
{
    struct stat file_info;
    struct dirent *p = NULL;
    
    DIR *dp = opendir(path);
    if(dp == NULL)
    {
        perror("opendir failed");
        return -1;
    }

    while((p = readdir(dp)) != NULL)
    {
            stat(p->d_name, &file_info);
            printf("d_name: %s\n", p->d_name);

            printf("File type:                ");
           switch (file_info.st_mode & S_IFMT) 
           {
                case S_IFBLK:  printf("block device\n");               break;
                case S_IFCHR:  printf("character device\n");      break;
                case S_IFDIR:  printf("directory\n");                       break;
                case S_IFIFO:  printf("FIFO/pipe\n");                     break;
                case S_IFLNK:  printf("symlink\n");                        break;
                case S_IFREG:  printf("regular file\n");                 break;
                case S_IFSOCK: printf("socket\n");                        break;
                default:       printf("unknown?\n");                          break;
           }
    }
    closedir(dp);
    return 0;
}
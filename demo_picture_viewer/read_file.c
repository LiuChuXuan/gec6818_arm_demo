#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

int main(void)
{

    int fd = open("a", O_RDONLY);
    if(fd == -1)
    {
        perror("open");
        return -1;
    }

    int fd_b = open("b",O_RDWR | O_CREAT);
    if(fd_b == -1)
    {
        perror("open");
        return -1;
    }

    lseek(fd, 7, SEEK_SET);
    char buf;
    int ret;
    while(1)
    {
        ret = read(fd, &buf, 1);
        if(ret == 0)
        {
            break;
        }
        if(ret == -1)
        {
            perror("read");
            return -1;            
        }
        write(fd_b, &buf, 1);
    }


    close(fd);
    close(fd_b);
    return 0;
}
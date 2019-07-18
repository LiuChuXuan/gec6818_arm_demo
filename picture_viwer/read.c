#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int fd = open(argv[1], O_RDONLY);
    if(fd == -1)
    {
        perror("open bmp failed");
        return -1;
    }

    int fd2 = open("copy_",O_RDWR | O_CREAT);
    {
        if(fd == -1)
        {
            perror("open copy_ failed");
            return -1;
        }
    }

    int ret = 0;
    char *a = (char*)malloc(1*sizeof(char));
    memset(a, 0, sizeof(a));
    

        ret = read(fd, a, sizeof(char));
        
        write(fd2, a, sizeof(char));

    printf("%s\n",argv[1]);
    printf("%d\n",*a);
    printf("%c\n",*a);

    return 0;
}

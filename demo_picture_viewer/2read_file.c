#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
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

    char buf[10];
    memset(buf, 0, sizeof(buf));

    int ret = read(fd, &buf, 10);

    printf("ret = %d\n",ret);

    int i = 0;
    for(i = 0; i < 10; i++)
    {
        printf("%c\t",buf[i]);
    }
    printf("\n");

    close(fd);
    return 0;
}
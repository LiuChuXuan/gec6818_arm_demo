#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

struct BITMAPINFOHEADER  
{   
    u_int32_t biSize;   
    u_int32_t biWidth;   
    u_int32_t biHeight;   
    u_int16_t biPlanes;   
    u_int16_t biBitCount;   
    u_int32_t biCompression;   
    u_int32_t biSizeImage;   
    u_int32_t biXPelsPerMeter;   
    u_int32_t biYPelsPerMeter;   
    u_int32_t biClrUsed;   
    u_int32_t biClrImportant;   
};  


int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("argc != 2");
        return -1;
    }

    struct BITMAPINFOHEADER bmp;
    memset(&bmp, 0, 40);

    int fd_bmp = open(argv[1], O_RDONLY);
    if(fd_bmp == -1)
    {
        perror("open bmp failed");
        return -1;
    }

    lseek(fd_bmp,  14,  SEEK_SET);
    read(fd_bmp, &bmp, 40);

    printf("%d, %d\n", bmp.biWidth, bmp.biHeight);

    
    return 0;
}
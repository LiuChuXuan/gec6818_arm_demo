#include "main.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("argv != 2");
        return -1;
    }
    
    show_bmp(argv[1],100, 100);

    return 0;
}
#include "main.h"

int main(void)
{
    int x,y;
    show_bmp("./ui_img/menu.bmp", 0, 0);

    // h > 700退出程序

    // x < 700 && y < 240 电子相册
    get_xy_release(&x, &y);
    printf("aaaa:(%d,%d)\n",x,y);
    if(x < 700 && y < 240)
    {

        printf("bbbb:(%d,%d)\n",x,y);
        picture_viewer("./img");
    }

    // h < 700 && x > 240 2048


    return 0;
}
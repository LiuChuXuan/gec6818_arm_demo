#include "picture_viewer.h"

int picture_viewer(char *path)
{
    if(path == NULL)
    {
        perror("picture_viewer path == NULL");
        return -1;
    }

    node_t img_list = NULL;
    list_create(&img_list);

    //递归地扫描path路径下的所有文件夹里的所有图片
    scan_picture(path, img_list);

    int picture_num = list_length(img_list);
    printf("picture amount: %d\n",picture_num);

    int count = 1;
	show_bmp_over((char *)(list_get_node(img_list, count)->item), 0, 0);
    printf("Display the first picture\n");

    int ret = 0;
    while(1)
    {
        ret = ch_img();
        if(ret == QUIT)
        {
            printf("quit picture viwer!\n");
            list_destroy(&img_list);
            return 0;
        }
        //向左滑动或者在屏幕右侧点击的时候切换到下一张图片
		else if(ret == MOVE_LEFT || ret > 400)
		{
            count++;
            if(count > picture_num)
            {
                count = 1;
            }
			show_bmp_over((char *)(list_get_node(img_list, count)->item), 0, 0);
            printf("Next:(no. %d)\n\n",count);
        }
        //向右滑动或者在屏幕左侧点击的时候切换到上一张图片
        else if(ret == MOVE_RIGHT || ret <= 400)
		{
            count--;
            if(count <= 0)
            {
                count = picture_num;
            }
			show_bmp_over((char *)(list_get_node(img_list, count)->item), 0, 0);
            printf("Previous:(no. %d)\n\n",count);
		}
    }

    list_destroy(&img_list);
    return 0;
}

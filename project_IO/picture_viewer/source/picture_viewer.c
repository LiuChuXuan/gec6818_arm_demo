#include "picture_viewer.h"

//相册的主程序
int picture_viewer(sc_info_t screen, char *path)
{
	if (path == NULL)
	{
		perror("picture_viewer path == NULL");
		return -1;
	}

	node_t img_list = NULL;
	list_create(&img_list);

	//递归地扫描path路径下的所有文件夹里的所有图片
	int ret = scan_picture(path, img_list);
	if (ret != 0)
	{
		list_destroy(&img_list);
		return -1;
	}

	int picture_num = list_length(img_list);
	printf("picture amount: %d\n", picture_num);

	int count = 1;
	show_bmp_over(screen, (char *)(list_get_node(img_list, count)->item), 0, 0);
	printf("Display the first picture\n");

	while (1)
	{
		ret = ch_img();
		if (ret == QUIT)
		{
			printf("quit picture viwer!\n");
			list_destroy(&img_list);
			return 0;
		}
		//向左滑动或者在屏幕右侧点击的时候切换到下一张图片
		else if (ret == MOVE_LEFT || ret > 400)
		{
			count++;
			if (count > picture_num)
			{
				count = 1;
			}
			show_bmp_over(screen, (char *)(list_get_node(img_list, count)->item), 0, 0);
			printf("Next:(no. %d)\n\n", count);
		}
		//向右滑动或者在屏幕左侧点击的时候切换到上一张图片
		else if (ret == MOVE_RIGHT || ret <= 400)
		{
			count--;
			if (count <= 0)
			{
				count = picture_num;
			}
			show_bmp_over(screen, (char *)(list_get_node(img_list, count)->item), 0, 0);
			printf("Previous:(no. %d)\n\n", count);
		}
	}

	list_destroy(&img_list);
	return 0;
}

//判断是左还是右还是点击，然后切换显示图片
int ch_img(void)
{
	int x0 = 0, y0 = 0, x1 = 0, y1 = 0;

	//获取按下和松开时的坐标
	int ret = get_xy(&x0, &y0, &x1, &y1);
	if (ret != 0)
	{
		perror("ch_img get xy error!");
		return -1;
	}

	printf("(%d,%d)\n", x0, x1);

	//从屏幕边缘滑动，退出相册
	if ((x0 < 15) && (x1 > 50))
	{
		printf("quit l : (%d,%d)\n", x0, x1);
		return QUIT;
	}
	else if ((x0 > 785) && (x1 < 750))
	{
		printf("quit r : (%d,%d)\n", x0, x1);
		return QUIT;
	}

	else
	{
		if (x0 > x1)
		{
			return MOVE_LEFT;
		}
		else if (x0 < x1)
		{
			return MOVE_RIGHT;
		}
		//如果x0 == x1 则是点击，返回x的值
		else
		{
			return x0;
		}
	}
}

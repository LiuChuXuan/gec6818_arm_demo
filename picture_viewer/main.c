#include "main.h"

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        perror("argc != 2");
        return -1;
    }

    node_t picture_link = NULL;
    link_create(&picture_link);

    scan_picture(argv[1], picture_link);

    int picture_num = link_length(picture_link);
    int count = 0;
    int x,y;
    while(1)
	{
		get_xy(&x, &y);
		printf("(%d,\t%d)\n",  x, y);

		if(x > 400)
		{
            count++;
            if(count > picture_num)
            {
                count = 1;
            }
			show_bmp(node_get(picture_link, count)->item, 0, 0);
		}
        else
		{
            count--;
            if(count <= 0)
            {
                count = picture_num;
            }
			show_bmp(node_get(picture_link, count)->item, 0, 0);
		}
	}


    link_destroy(&picture_link);

    return 0;
}
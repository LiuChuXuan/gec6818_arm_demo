#include "main.h"

int main(int argc, char **argv)
{
	//判断一下主函数传参够不够
	if(argc != 3)
	{
		printf("argc != 3\n");
		return -1;
	}


	scan_dir(argv[1],argv[2]);


    return 0;
}
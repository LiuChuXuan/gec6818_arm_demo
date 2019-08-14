#include "main.h"

int main(int argc, char **argv)
{

	//判断一下主函数传参够不够
	if(argc != 3)
	{
		printf("argc != 3\n");
		return -1;
	}

	//定义一个线程池
	thread_pool *pool=calloc(1, sizeof(thread_pool)); 
	
	//初始化线程池
	init_pool(pool, 5);

	//扫描文件夹将所有文件添加到任务队列
	scan_dir(pool, argv[1],argv[2]);
	
	//销毁线程池
	destroy_pool(pool);

    return 0;
}
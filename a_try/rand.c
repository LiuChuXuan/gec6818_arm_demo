#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char **argv)
{
	int num;
	while(1)
	{
		num = rand()%10; //0~9随机数
		printf("%d\n", num);
		
		sleep(1);
	}
	
	
	return 0;
}
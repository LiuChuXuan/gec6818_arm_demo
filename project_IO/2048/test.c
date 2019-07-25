#include <stdio.h>
#include "list_2048.h"
#include "imshow_2048.h"


int main(void)
{
    int direction = 0, score = 0;

//1.创建新游戏
    list_2048_t new_game = NULL;
    list_2048_create(&new_game);
    list_2048_randcreate(new_game);
    list_2048_randcreate(new_game);

//2.显示开局棋盘
    list_2048_travel(new_game);
    imshow_2048(new_game);


//3.响应上下左右交互
    score = move(new_game);

    list_2048_destroy(&new_game);
    return 0;
}
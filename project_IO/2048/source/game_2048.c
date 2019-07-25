#include "game_2048.h"

int game_2048(void)
{
    int direction = 0, score = 0;
    

//1.创建新游戏
    list_2048_t game = NULL;
    list_2048_create(&game);
    list_2048_randcreate(game);
    list_2048_randcreate(game);

//2.显示开局棋盘
    list_2048_travel(game);
    imshow_2048(game);

//3.响应上下左右交互
    while(1)
    {
        direction = get_direction();
        switch(direction)
        {
            case MOVE_ABOVE: 
                score += list_2048_move_above(game);
                list_2048_randcreate(game);
                printf("above, score = %d\n", score);
                list_2048_travel(game);
                imshow_2048(game);
                break;
            case MOVE_BELOW:
                score += list_2048_move_below(game);
                list_2048_randcreate(game);
                printf("below, score = %d\n", score);
                list_2048_travel(game);
                imshow_2048(game);
                break;
            case MOVE_LEFT:
                score += list_2048_move_left(game);
                list_2048_randcreate(game);
                printf("left, score = %d\n", score);
                list_2048_travel(game);
                imshow_2048(game);
                break;
            case MOVE_RIGHT:
                score += list_2048_move_right(game);
                list_2048_randcreate(game);
                printf("right, score = %d\n", score);
                list_2048_travel(game);
                imshow_2048(game);
                break;
            case QUIT_2048:
                list_2048_destroy(&game);
                return 0;
            default:
                printf("move while switch default");
                break;
        }
    }

    list_2048_destroy(&game);

    return 0;
}
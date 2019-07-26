#include "game_2048.h"

int game_2048(void)
{
    int direction = 0, ret = 0;

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
                game->score += list_2048_move_above(game);
                printf("above, game->score = %d\n", game->score);
                break;
            
            case MOVE_BELOW:
                game->score += list_2048_move_below(game);
                printf("below, game->score = %d\n", game->score);
                break;

            case MOVE_LEFT:
                game->score += list_2048_move_left(game);
                printf("left, game->score = %d\n", game->score);
                break;

            case MOVE_RIGHT:
                game->score += list_2048_move_right(game);
                printf("right, game->score = %d\n", game->score);
                break;

            case QUIT:
                list_2048_destroy(&game);
                return 0;

            default:
                printf("move while switch default\n");
                break;
        }

        ret = list_2048_randcreate(game);
        if(ret == -2)
        {
            perror("game over\n");
            break;
        }
        list_2048_travel(game);
        imshow_2048(game);
    }

    list_2048_destroy(&game);

    return 0;
}
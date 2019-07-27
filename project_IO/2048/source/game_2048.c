#include "game_2048.h"

int game_2048(void)
{
    int direction = 0, ret = 0;
    node_t list_node_temp = NULL;
    list_2048_t list_2048_temp = NULL;

//创建一个链表用于保存游戏进度，用于撤销
    node_t game_log = NULL;
    list_create(&game_log);

//1.创建新游戏
    list_2048_t game = NULL;
    list_2048_create(&game);
    ret = list_2048_load(game);
    if(ret == -1)
    {
        list_2048_randcreate(game);
        list_2048_randcreate(game);
    }

    list_2048_record(game_log, game);

//2.显示开局棋盘
    list_2048_travel(game);
    imshow_2048(game);

//3.响应上下左右交互
    while(1)
    {
        direction = get_direction_2048();
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
                list_destroy(&game_log);
                list_2048_destroy(&game);
                return 0;
            
            case GAME_2048_RESTART:
                list_init(game_log);
                list_2048_init(game);
                list_2048_randcreate(game);
                break;
            
            case GAME_2048_REVOKE:
                ret = list_length(game_log);
                if(ret <= 1)
                {
                    continue;
                }

                list_delete_node(game_log, -1);
                list_node_temp = list_get_node(game_log, -1);
                list_2048_temp = (list_2048_t)(list_node_temp->item);
                memcpy(game, list_2048_temp, sizeof(list_2048));
                list_2048_travel(game);
                imshow_2048(game);
                continue;

            default:
                printf("move while switch default\n");
                break;
        }

        ret = list_2048_randcreate(game);
        if(ret == -2)
        {
            perror("game over");
            break;
        }

        list_2048_record(game_log, game);

        ret = list_2048_save(game);
        if(ret == -1)
        {
            break;            
        }

        list_2048_travel(game);
        imshow_2048(game);
    }

    list_destroy(&game_log);
    list_2048_destroy(&game);

    return 0;
}
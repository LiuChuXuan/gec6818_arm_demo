#include "game_2048.h"

int game_2048(void)
{
    int direction = 0, ret = 0;
    node_t list_node_temp = NULL;
    list_2048_t list_2048_temp = NULL;

//创建一个链表用于保存游戏进度，用于撤销
    node_t game_log = NULL;
    list_create(&game_log);

//1.创建新游戏game结构体
    list_2048_t game = NULL;
    list_2048_create(&game);
    
//判断硬盘有没有可以加载的游戏。
    ret = list_2048_load(game);
    if(ret == -1)
    {
        //如果没有，则开始新游戏，开局随机生成两个2
        list_2048_randcreate(game);
        list_2048_randcreate(game);
    }

    //将当前游戏操作步骤，即game结构体，插入到game_log这个链表中
    list_2048_record(game_log, game);

//2.显示开局棋盘
    list_2048_travel(game);
    imshow_2048(game);

//3.响应上下左右交互
    while(1)
    {
        //获取触摸屏状态，判断上下左右还是点击撤销、重来、退出
        direction = get_direction_2048();
        switch(direction)
        {
            //向上滑
            case MOVE_ABOVE: 
                game->score += list_2048_move_above(game);
                printf("above, game->score = %d\n", game->score);
                break;
            //向下滑
            case MOVE_BELOW:
                game->score += list_2048_move_below(game);
                printf("below, game->score = %d\n", game->score);
                break;
            //向左滑
            case MOVE_LEFT:
                game->score += list_2048_move_left(game);
                printf("left, game->score = %d\n", game->score);
                break;
            //向右滑
            case MOVE_RIGHT:
                game->score += list_2048_move_right(game);
                printf("right, game->score = %d\n", game->score);
                break;
            //退出游戏
            case QUIT:
                list_destroy(&game_log);
                list_2048_destroy(&game);
                return 0;
            //重新开始游戏
            case GAME_2048_RESTART:
                list_init(game_log);
                list_2048_init(game);
                list_2048_randcreate(game);
                break;
            //撤销到上一步
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

        //每次上下左右移动过后，随机生成一个2，如果生成不了了，就判断游戏结束。
        ret = list_2048_randcreate(game);
        if(ret == -2)
        {
            perror("game over");
            show_bmp("./ui_img/gameover_2048.bmp", 0, 0);
            sleep(2);
            break;
        }

        //将当前游戏操作步骤，即game结构体，插入到game_log这个链表中
        list_2048_record(game_log, game);

        //将游戏保存到硬盘，即把内存中的数据(game结构体)写入硬盘
        ret = list_2048_save(game);
        if(ret == -1)
        {
            break;            
        }

        list_2048_travel(game); //游戏显示到终端
        imshow_2048(game);      //游戏显示到液晶屏
    }

    list_destroy(&game_log);
    list_2048_destroy(&game);

    return 0;
}
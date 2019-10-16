#include "list_2048.h"

//将item插入到二维数组game->item[y][x]中
int list_2048_set(list_2048_t game, int x, int y, elen_2048_t item)
{
    if (game != NULL)
    {
        if (x < 0 || x > 3 || y < 0 || y > 3)
        {
            perror("list 2048 set failed");
            return -1;
        }
        if (game->item[y][x] == 0 && item != 0)
        {
            game->len++;
        }
        else if (game->item[y][x] != 0 && item == 0)
        {
            game->len--;
        }
        game->item[y][x] = item;
    }
    else
    {
        perror("list 2048 init game == NULL");
    }
    return 0;
}

//随机将game其中一个为零的设置成2
int list_2048_randcreate(list_2048_t game)
{
    if (game == NULL)
    {
        perror("2048 randcreate == NULL");
        return -1;
    }

    if (game->len == 16)
    {
        perror("2048 randcreate : FULL");
        return -2;
    }

    srand((unsigned)time(NULL));

    int x = rand() % 4;
    int y = rand() % 4;

    while (game->item[y][x] != 0)
    {
        x = rand() % 4;
        y = rand() % 4;
    }

    list_2048_set(game, x, y, 2);

    return 0;
}

list_2048_t list_2048_init(list_2048_t game)
{
    if (game != NULL)
    {
        memset(game, 0, sizeof(list_2048));
    }
    else
    {
        perror("list 2048 init == NULL");
    }
    return game;
}

//创建game结构体
list_2048_t list_2048_create(list_2048_t *new_list)
{
    *new_list = (list_2048_t)malloc(sizeof(list_2048));
    list_2048_init(*new_list);
    return *new_list;
}

//遍历game结构体，输出到终端
int list_2048_travel(list_2048_t game)
{
    if (game != NULL)
    {
        int x = 0, y = 0;
        printf("|-------------------------------|\n");
        for (y = 0; y < 4; y++)
        {
            printf("|");
            for (x = 0; x < 4; x++)
            {
                printf("    %d\t", game->item[y][x]);
            }
            printf("|\n");
        }
        printf("|-------------------------------|\n\n");
    }
    return 0;
}

//销毁game结构体，释放资源
list_2048_t list_2048_destroy(list_2048_t *game)
{
    free(*game);
    *game = NULL;
    return *game;
}

//return score
int list_2048_move_left(list_2048_t game)
{
    if (game == NULL)
    {
        perror("2048 move left game == NULL");
        return -1;
    }

    int x = 0, y = 0, count = 0, score = 0, flag = 0;

    for (y = 0; y < 4; y++)
    {
        //去零左移
        for (flag = 0, x = 0; x < 4;)
        {
            if (game->item[y][x] != 0)
            {
                if (flag == x)
                {
                    flag++;
                    x++;
                }
                else
                {
                    list_2048_set(game, flag, y, game->item[y][x]);
                    list_2048_set(game, x, y, 0);
                    flag++;
                    x = flag;
                }
            }
            else
            {
                x++;
            }
        }
        //同等合并
        for (x = 0; x < 3; x++)
        {
            if (game->item[y][x] == game->item[y][x + 1])
            {
                game->item[y][x] *= 2;
                score += game->item[y][x];
                for (count = x; count < 2; count++)
                {
                    list_2048_set(game, count + 1, y, game->item[y][count + 2]);
                }
                list_2048_set(game, 3, y, 0);
            }
        }
    }
    return score;
}

int list_2048_move_right(list_2048_t game)
{
    if (game == NULL)
    {
        perror("2048 move right game == NULL");
        return -1;
    }
    int x = 0, y = 0, count = 0, score = 0, flag = 0;

    for (y = 0; y < 4; y++)
    {
        //去零右移
        for (flag = 3, x = 3; x >= 0;)
        {
            if (game->item[y][x] != 0)
            {
                if (flag == x)
                {
                    flag--;
                    x--;
                }
                else
                {
                    list_2048_set(game, flag, y, game->item[y][x]);
                    list_2048_set(game, x, y, 0);
                    flag--;
                    x = flag;
                }
            }
            else
            {
                x--;
            }
        }
        //同等合并
        for (x = 3; x >= 0; x--)
        {
            if (game->item[y][x] == game->item[y][x - 1])
            {
                game->item[y][x] *= 2;
                score += game->item[y][x];
                for (count = x; count > 1; count--)
                {
                    list_2048_set(game, count - 1, y, game->item[y][count - 2]);
                }
                list_2048_set(game, 0, y, 0);
            }
        }
    }
    return score;
}

int list_2048_move_above(list_2048_t game)
{
    if (game == NULL)
    {
        perror("2048 move above game == NULL");
        return -1;
    }

    int x = 0, y = 0, count = 0, score = 0, flag = 0;

    for (x = 0; x < 4; x++)
    {
        //去零上移
        for (flag = 0, y = 0; y < 4;)
        {
            if (game->item[y][x] != 0)
            {
                if (flag == y)
                {
                    flag++;
                    y++;
                }
                else
                {
                    list_2048_set(game, x, flag, game->item[y][x]);
                    list_2048_set(game, x, y, 0);
                    flag++;
                    y = flag;
                }
            }
            else
            {
                y++;
            }
        }

        //同等合并
        for (y = 0; y < 3; y++)
        {
            if (game->item[y][x] == game->item[y + 1][x])
            {
                game->item[y][x] *= 2;
                score += game->item[y][x];
                for (count = y; count < 2; count++)
                {
                    list_2048_set(game, x, count + 1, game->item[count + 2][x]);
                }
                list_2048_set(game, x, 3, 0);
            }
        }
    }
    return score;
}

int list_2048_move_below(list_2048_t game)
{
    if (game == NULL)
    {
        perror("2048 move below game == NULL");
        return -1;
    }

    int x = 0, y = 0, count = 0, score = 0, flag = 0;

    for (x = 0; x < 4; x++)
    {
        //去零下移
        for (flag = 3, y = 3; y >= 0;)
        {
            if (game->item[y][x] != 0)
            {
                if (flag == y)
                {
                    flag--;
                    y--;
                }
                else
                {
                    list_2048_set(game, x, flag, game->item[y][x]);
                    list_2048_set(game, x, y, 0);
                    flag--;
                    y = flag;
                }
            }
            else
            {
                y--;
            }
        }

        //同等合并
        {
            for (y = 3; y >= 0; y--)
            {
                if (game->item[y][x] == game->item[y - 1][x])
                {
                    game->item[y][x] *= 2;
                    score += game->item[y][x];
                    for (count = y; count > 1; count--)
                    {
                        list_2048_set(game, x, count - 1, game->item[count - 2][x]);
                    }
                    list_2048_set(game, x, 0, 0);
                }
            }
        }
    }
    return score;
}

//将内存中的游戏信息(链表game_log)保存到硬盘
int list_2048_save(node_t game_log)
{
    if (game_log == NULL)
    {
        perror("2048 save game_log == NULL");
        return -1;
    }
    //准备将游戏保存到./save_2048这个文件里面
    FILE *fp = fopen("./save_2048", "w+");
    if (fp == NULL)
    {
        perror("fopen error");
        return -1;
    }

    int ret = 0;
    node_t temp = game_log->next;

    while (temp != game_log)
    {
        ret = fwrite(temp->item, sizeof(list_2048), 1, fp);
        if (ret != 1)
        {
            perror("game_log save error");
            return -1;
        }
        temp = temp->next;
    }

    fclose(fp);
    return 0;
}

//将硬盘的游戏信息加载到(结构体game和链表game_log)
int list_2048_load(node_t game_log, list_2048_t game)
{
    if (game_log == NULL)
    {
        perror("2048 save game_log == NULL");
        return -1;
    }
    if (game == NULL)
    {
        perror("2048 save game == NULL");
        return -1;
    }
    //打开游戏保存的那个文件
    FILE *fp = fopen("./save_2048", "r");
    if (fp == NULL)
    {
        perror("fopen error");
        return -2;
    }

    //链表初始化
    list_init(game_log);

    while (fread(game, sizeof(list_2048), 1, fp))
    {
        list_2048_record(game_log, game);
    }

    fclose(fp);
    return 0;
}

//将结构体game插入到链表game_log
int list_2048_record(node_t game_log, list_2048_t game)
{
    if (game == NULL)
    {
        perror("2048 log game == NULL");
        return -1;
    }
    if (game_log == NULL)
    {
        perror("2048 game log == NULL");
        return -1;
    }

    list_2048_t temp = NULL;
    list_2048_create(&temp);
    memcpy(temp, game, sizeof(list_2048));
    list_insert_node(game_log, temp, sizeof(list_2048), -1);
    return 0;
}

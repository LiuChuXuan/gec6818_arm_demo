#include "list_2048.h"

int list_2048_set(list_2048_t game, int x, int y, elen_2048_t item)
{
    if(game != NULL)
    {
        if(x<0 || x>3 || y<0 || y>3)
        {
            perror("list 2048 set failed\n");
            return -1;
        }
        if(game->item[y][x] == 0 && item != 0)
        {
            game->len++;
        }
        else if(game->item[y][x] != 0 && item == 0)
        {
            game->len--;
        }
        game->item[y][x] = item;
    }
    else
    {
        perror("list 2048 init game == NULL\n");
    }
    return 0; 
}

int list_2048_randcreate(list_2048_t game)
{
    if(game == NULL)
    {
        perror("2048 randcreate == NULL\n");
        return -1;
    }

    if(game->len == 16)
    {
        perror("2048 randcreate : FULL\n");
        return -2;
    }

    srand((unsigned)time(NULL));

    int x = rand() % 4;
    int y = rand() % 4;
    
    while(game->item[y][x] != 0)
    {
        x = rand() % 4;
        y = rand() % 4;
    }

    list_2048_set(game, x, y, 2);

    return 0;
}

list_2048_t list_2048_init(list_2048_t game)
{
    if(game != NULL)
    {
        memset(game, 0, sizeof(list_2048));
    }
    else
    {
        perror("list 2048 init == NULL\n");
    }
    return game;
}

list_2048_t list_2048_create(list_2048_t *new_list)
{
    *new_list = (list_2048_t)malloc(sizeof(list_2048));
    list_2048_init(*new_list);
    return *new_list;
}

int list_2048_travel(list_2048_t game)
{
    if(game != NULL)
    {
        int x = 0, y = 0;
        printf("|--------------------|\n");
        for(y = 0; y < 4; y++)
        {
            printf("|");
            for(x = 0; x < 4; x++)
            {
                printf("  %d  ", game->item[y][x]);
            }
            printf("|\n");
        }
        printf("|--------------------|\n\n");
    }
    return 0;
}

list_2048_t list_2048_destroy(list_2048_t *game)
{
    free(*game);
    *game = NULL;
    return *game;
}

//return score
int list_2048_move_left(list_2048_t game)
{
    if(game == NULL)
    {
        perror("2048 move left game == NULL\n");
        return -1;
    }

    int x = 0, y = 0, count = 0, score = 0, flag = 0;

    for(y = 0; y < 4; y++)
    {
        //去零左移
        for(flag = 0, x = 0; x < 4; )
        {
            if(game->item[y][x] != 0)
            {
                if(flag == x)
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
        for(x = 0; x < 3; x++)
        {
            if(game->item[y][x] == game->item[y][x+1])
            {
                game->item[y][x] *= 2;
                score += game->item[y][x];
                for(count = x; count < 2; count++)
                {
                    list_2048_set(game, count+1, y, game->item[y][count+2]);
                }
                list_2048_set(game, 3, y, 0);
            }
        }
    }
    return score;
}

int list_2048_move_right(list_2048_t game)
{
    if(game == NULL)
    {
        perror("2048 move right game == NULL\n");
        return -1;
    }
    int x = 0, y = 0, count = 0, score = 0, flag = 0;
    
    for(y = 0; y < 4; y++)
    {
        //去零右移
        for(flag = 3, x = 3; x >= 0; )
        {
            if(game->item[y][x] != 0)
            {
                if(flag == x)
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
        {
            for(x = 3; x >= 0; x--)
            {
                if(game->item[y][x] == game->item[y][x-1])
                {
                    game->item[y][x] *= 2;
                    score += game->item[y][x];
                    for(count = x; count > 1; count--)
                    {
                        list_2048_set(game, count-1, y, game->item[y][count-2]);
                    }
                    list_2048_set(game, 0, y, 0);
                }
            }


        }
    }    
    return score;
}

int list_2048_move_above(list_2048_t game)
{
    if(game == NULL)
    {
        perror("2048 move above game == NULL\n");
        return -1;
    }

    int x = 0, y = 0, count = 0, score = 0, flag = 0;

    for(x = 0; x < 4; x++)
    {
        //去零上移
        for(flag = 0, y = 0; y < 4; )
        {
            if(game->item[y][x] != 0)
            {
                if(flag == y)
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
        for(y = 0; y < 3; y++)
        {
            if(game->item[y][x] == game->item[y+1][x])
            {
                game->item[y][x] *= 2;
                score += game->item[y][x];
                for(count = y; count < 2; count++)
                {
                    list_2048_set(game, x, count+1, game->item[count+2][x]);
                }
                list_2048_set(game, x, 3, 0);
            }
        }
    }
    return score;
}

int list_2048_move_below(list_2048_t game)
{
    if(game == NULL)
    {
        perror("2048 move below game == NULL\n");
        return -1;
    }

    int x = 0, y = 0, count = 0, score = 0, flag = 0;
    
    for(x = 0; x < 4; x++)
    {
        //去零下移
        for(flag = 3, y = 3; y >= 0; )
        {
            if(game->item[y][x] != 0)
            {
                if(flag == y)
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
            for(y = 3; y >= 0; y--)
            {
                if(game->item[y][x] == game->item[y-1][x])
                {
                    game->item[y][x] *= 2;
                    score += game->item[y][x];
                    for(count = y; count > 1; count--)
                    {
                        list_2048_set(game, x, count-1, game->item[count-2][x]);
                    }
                    list_2048_set(game, x, 0, 0);
                }
            }
        }

    }    
    return score;
}

int list_2048_save(list_2048_t game)
{
    FILE *fp = fopen("./save_2048","w");
    if(fp == NULL)
    {
        perror("fopen error\n");
        return -1;
    }

    int ret = fwrite(game, sizeof(list_2048), 1, fp);
    if(ret != 1)
    {
        perror("save game error\n");
        return -1;
    }

    fclose(fp);
    return 0;
}

int list_2048_load(list_2048_t game)
{
    FILE *fp = fopen("./save_2048","r");
    if(fp == NULL)
    {
        perror("fopen error\n");
        return -1;
    }

    int ret = fread(game, sizeof(list_2048), 1, fp);
    if(ret != 1)
    {
        perror("load game error\n");
        return -1;
    }

    fclose(fp);
    return 0;
}
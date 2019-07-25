#include "list_2048.h"


list_2048_t list_2048_randcreate(list_2048_t head)
{
    if(head == NULL)
    {
        perror("2048 randcreate == NULL");
        return head;
    }
    srand((unsigned)time(NULL));
    int a = rand() % 4;
    int b = rand() % 4;
    while(head->item[a][b] != 0)
    {
        a = rand() % 4;
        b = rand() % 4;
    }
    head->item[a][b] = 2;
    return head;
}

list_2048_t list_2048_init(list_2048_t head)
{
    if(head != NULL)
    {
        memset(head, 0, sizeof(list_2048));
    }
    else
    {
        perror("list 2048 init head == NULL");
    }
    return head;
}

list_2048_t list_2048_create(list_2048_t *new_list)
{
    *new_list = (list_2048_t)malloc(sizeof(list_2048));
    list_2048_init(*new_list);
    return *new_list;
}

list_2048_t list_2048_set(list_2048_t head, int x, int y, elen_2048_t item)
{
    if(head != NULL)
    {
        if(x<1 || x>4 || y<1 || y>4)
        {
            perror("list 2048 set failed");
            return head;
        }
        x--;
        y--;
        if(head->item[y][x] == 0 && item != 0)
        {
            head->x_len[y]++;
            head->y_len[x]++;
        }
        else if(head->item[y][x] != 0 && item == 0)
        {
            head->x_len[y]--;
            head->y_len[x]--;
        }
        head->item[y][x] = item;
    }
    else
    {
        perror("list 2048 init head == NULL");
    }
    return head; 
}

int list_2048_travel(list_2048_t head)
{
    if(head != NULL)
    {
        int x = 0, y = 0;
        printf("|--------------------|\n");
        for(y = 0; y < 4; y++)
        {
            printf("|");
            for(x = 0; x < 4; x++)
            {
                printf("  %d  ", head->item[y][x]);
            }
            printf("|\n");
        }
        printf("|--------------------|\n\n");
    }
    return 0;
}

list_2048_t list_2048_destroy(list_2048_t *head)
{
    free(*head);
    *head = NULL;
    return *head;
}

//return score
int list_2048_move_left(list_2048_t head)
{
    if(head == NULL)
    {
        perror("2048 move left head == NULL");
        return -1;
    }

    int x = 0, y = 0, count = 0, score = 0, flag = 0;

    for(y = 0; y < 4; y++)
    {
        //去零左移
        for(flag = 0, x = 0; x < 4; )
        {
            if(head->item[y][x] != 0)
            {
                if(flag == x)
                {
                    flag++;
                    x++;
                }
                else 
                {
                    head->item[y][flag] = head->item[y][x];
                    head->item[y][x] = 0;
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
            if(head->item[y][x] == head->item[y][x+1])
            {
                head->item[y][x] *= 2;
                score += head->item[y][x];
                for(count = x; count < 2; count++)
                {
                    head->item[y][count+1] = head->item[y][count+2];
                }
                head->item[y][3] = 0;
            }
        }
    }
    return score;
}

int list_2048_move_right(list_2048_t head)
{
    if(head == NULL)
    {
        perror("2048 move right head == NULL");
        return -1;
    }
    int x = 0, y = 0, count = 0, score = 0, flag = 0;
    
    for(y = 0; y < 4; y++)
    {
        //去零右移
        for(flag = 3, x = 3; x >= 0; )
        {
            if(head->item[y][x] != 0)
            {
                if(flag == x)
                {
                    flag--;
                    x--;
                }
                else
                {
                    head->item[y][flag] = head->item[y][x];
                    head->item[y][x] = 0;
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
                if(head->item[y][x] == head->item[y][x-1])
                {
                    head->item[y][x] *= 2;
                    score += head->item[y][x];
                    for(count = x; count > 1; count--)
                    {
                        head->item[y][count-1] = head->item[y][count-2];
                    }
                    head->item[y][0] = 0;
                }
            }


        }
    }    
    return score;
}

int list_2048_move_above(list_2048_t head)
{
    if(head == NULL)
    {
        perror("2048 move above head == NULL");
        return -1;
    }

    int x = 0, y = 0, count = 0, score = 0, flag = 0;

    for(x = 0; x < 4; x++)
    {
        //去零上移
        for(flag = 0, y = 0; y < 4; )
        {
            if(head->item[y][x] != 0)
            {
                if(flag == y)
                {
                    flag++;
                    y++;
                }
                else
                {
                    head->item[flag][x] = head->item[y][x];
                    head->item[y][x] = 0;
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
            if(head->item[y][x] == head->item[y+1][x])
            {
                head->item[y][x] *= 2;
                score += head->item[y][x];
                for(count = y; count < 2; count++)
                {
                    head->item[count+1][x] = head->item[count+2][x];
                }
                head->item[3][x] = 0;
            }
        }
    }
    return score;
}

int list_2048_move_below(list_2048_t head)
{
    if(head == NULL)
    {
        perror("2048 move below head == NULL");
        return -1;
    }

    int x = 0, y = 0, count = 0, score = 0, flag = 0;
    
    for(x = 0; x < 4; x++)
    {
        //去零下移
        for(flag = 3, y = 3; y >= 0; )
        {
            if(head->item[y][x] != 0)
            {
                if(flag == y)
                {
                    flag--;
                    y--;
                }
                else
                {
                    head->item[flag][x] = head->item[y][x];
                    head->item[y][x] = 0;
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
                if(head->item[y][x] == head->item[y-1][x])
                {
                    head->item[y][x] *= 2;
                    score += head->item[y][x];
                    for(count = y; count > 1; count--)
                    {
                        head->item[count-1][x] = head->item[count-2][x];
                    }
                    head->item[0][x] = 0;
                }
            }
        }

    }    
    return score;
}


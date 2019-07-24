#include "list_2048.h"

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

list_2048_t list_2048_set(list_2048_t head, int x, int y, element_t item)
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

//return score
int list_2048_move_left(list_2048_t head)
{
    int score = 0;
    int flag = 0;
    int x = 0, y = 0, count = 0;

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
        ///* 
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
        //*/
    }
    return score;
}

list_2048_t list_2048_destroy(list_2048_t *head)
{
    free(*head);
    *head = NULL;
    return *head;
}
#include <stdio.h>
#include "list_2048.h"

int main(void)
{
    list_2048_t new_game = NULL;
    list_2048_create(&new_game);

    list_2048_set(new_game,1,4,2);
    list_2048_set(new_game,3,4,2);
    list_2048_set(new_game,4,4,2);


    list_2048_travel(new_game);

    list_2048_move_left(new_game);

    list_2048_travel(new_game);


    return 0;
}
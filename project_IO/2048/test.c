#include <stdio.h>
#include "list_2048.h"
#include "imshow_2048.h"


int main(void)
{
    int direction = 0, score = 0;

    list_2048_t new_game = NULL;
    list_2048_create(&new_game);
    list_2048_randcreate(new_game);
    list_2048_randcreate(new_game);

    list_2048_travel(new_game);

    score = move(new_game);

    list_2048_destroy(&new_game);
    return 0;
}
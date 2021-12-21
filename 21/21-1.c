#include <stdlib.h>
#include <stdio.h>
#include "../check_alloc.h"

#define P1_START 4
#define P2_START 8

#define MIN(x, y) ((x) < (y) ? (x) : (y))

typedef struct
{
    int position;
    int score;
} PLAYER;

PLAYER *new_player(int initial_position)
{
    PLAYER *p_player = calloc(1, sizeof(PLAYER));
    CHECK_ALLOC(p_player);
    p_player->position = initial_position;
    return p_player;
}

void move_position(PLAYER *player, int move)
{
    int next_pos = (player->position + move) % 10;
    player->position = next_pos == 0 ? 10 : next_pos;
    player->score += player->position;
}

typedef struct
{
    int value;
    int rolls;
} DICE;

#define NEW_DICE() calloc(1, sizeof(DICE))

int roll_dice(DICE *dice)
{
    dice->value = dice->value % 100 + 1;
    dice->rolls++;
    return dice->value;
}
int roll_3(DICE *dice)
{
    int value = roll_dice(dice);
    value += roll_dice(dice);
    value += roll_dice(dice);
    return value;
}

int main(int argc, char const *argv[])
{
    DICE *p_dice = NEW_DICE();
    PLAYER *p_p1 = new_player(P1_START);
    PLAYER *p_p2 = new_player(P2_START);
    while (1)
    {
        int value = roll_3(p_dice);
        move_position(p_p1, value);
        if (p_p1->score >= 1000)
            break;
        value = roll_3(p_dice);
        move_position(p_p2, value);
        if (p_p2->score >= 1000)
            break;
    }
    printf("END RESULT %d\n", p_dice->rolls * MIN(p_p1->score, p_p2->score));
    return 0;
}

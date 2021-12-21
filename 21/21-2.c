#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// I needed some help for this one. Couldn't be bothered to implement a cache etc.
// I calculated the DICE array, but struggled getting the total wins. Instead of
// multiplying as I went up, I tried to multiply when a result occurred but
// obviously this was the wrong way to think about it.

#define P1_START 6
#define P2_START 1

typedef struct
{
    int position;
    int score;
} PLAYER;

PLAYER clone_player(PLAYER player)
{
    PLAYER new_player = {player.position, player.score};
    return new_player;
}

void move_position(PLAYER *player, int move)
{
    int next_pos = (player->position + move) % 10;
    player->position = next_pos == 0 ? 10 : next_pos;
    player->score += player->position;
}

typedef unsigned long long NUMBER;

typedef enum
{
    P1,
    P2
} TURN;

typedef struct
{
    NUMBER p1_wins;
    NUMBER p2_wins;
} SCORE;

// PRE-CALCULATED OCCURRENCES OF TOTALS FOR ROLLING 3 DICE
int DICE[] = {1, 3, 6, 7, 6, 3, 1};

// repeat TRACKS THE AMOUNT OF POSSIBLE COMBINATIONS LEADING TO A WIN,
// THIS REDUCES THE SEARCH AREA GREATLY BY IGNORING PATHS TO THE SAME WIN.
void iter_rolls(PLAYER p1, PLAYER p2, TURN turn, NUMBER repeat, SCORE *score)
{
    for (int i = 0; i < 7; i++)
    {
        int value = i + 3;
        PLAYER tmp;
        if (turn == P1)
            tmp = clone_player(p1);
        else
            tmp = clone_player(p2);
        move_position(&tmp, value);
        if (tmp.score < 21)
        {
            if (turn == P1)
                iter_rolls(tmp, p2, P2, repeat * DICE[i], score);
            else
                iter_rolls(p1, tmp, P1, repeat * DICE[i], score);
        }
        else
        {
            if (turn == P1)
                score->p1_wins += repeat * DICE[i];
            else
                score->p2_wins += repeat * DICE[i];
        }
    }
}

int main(int argc, char const *argv[])
{
    PLAYER p1 = {P1_START, 0};
    PLAYER p2 = {P2_START, 0};
    SCORE *p_score = malloc(sizeof(SCORE));
    iter_rolls(p1, p2, P1, 1, p_score);
    printf("WINNER SCORE %llu\n", p_score->p1_wins > p_score->p2_wins ? p_score->p1_wins : p_score->p2_wins);
    return 0;
}

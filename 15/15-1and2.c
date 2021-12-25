#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include "../check_alloc.h"

#define PART_2

#define TILE_SIZE 100
#ifdef PART_2
#define REPEAT_TILE 5
#define SPACE_SIZE (TILE_SIZE * REPEAT_TILE)
#else
#define SPACE_SIZE TILE_SIZE
#endif

typedef struct
{
    int risk;
    int min_risk_path;
    bool explored;
} CELL;

typedef CELL **SPACE;

#define VALID_POSITION(x, y) (x < SPACE_SIZE && x >= 0 && y < SPACE_SIZE && y >= 0)
#define END(x, y) (x == SPACE_SIZE - 1 && y == SPACE_SIZE - 1)

void print_space(SPACE space)
{
    for (size_t i = 0; i < SPACE_SIZE; i++)
    {
        for (size_t j = 0; j < SPACE_SIZE; j++)
        {
            // printf("%d(%c,%d)", space[i][j].risk, space[i][j].explored ? '*' : '.', space[i][j].min_risk_path);
            // printf("%d(%d)", space[i][j].risk, space[i][j].min_risk_path);
            printf("%d", space[i][j].risk);
            if (j % TILE_SIZE == TILE_SIZE - 1)
                putchar(' ');
        }
        putchar('\n');
        if (i % TILE_SIZE == TILE_SIZE - 1)
            putchar('\n');
    }
    putchar('\n');
}

void node_risk(SPACE space, int x, int y, int risk, bool *modified)
{
    if (!VALID_POSITION(x, y))
        return;
    risk += space[x][y].risk;
    if (risk < space[x][y].min_risk_path)
    {
        space[x][y].min_risk_path = risk;
        *modified = true;
    }
}

int main(int argc, char const *argv[])
{
    FILE *p_file = fopen("input", "r");
    CHECK_ALLOC(p_file);
    SPACE space = calloc(SPACE_SIZE, sizeof(CELL *));
    CHECK_ALLOC(space);
    for (size_t i = 0; i < SPACE_SIZE; i++)
    {
        space[i] = calloc(SPACE_SIZE, sizeof(CELL));
        CHECK_ALLOC(space[i]);
    }

    for (size_t i = 0; i < TILE_SIZE; i++)
    {
        char buf[BUFSIZ];
        fgets(buf, sizeof buf, p_file);
        for (size_t j = 0; j < TILE_SIZE; j++)
        {
            space[i][j].risk = buf[j] - '0';
            space[i][j].explored = false;
            space[i][j].min_risk_path = INT_MAX;
        }
    }

// EXPAND SPACE
#ifdef PART_2
    for (size_t i = 0; i < SPACE_SIZE; i++)
    {
        for (size_t j = 0; j < SPACE_SIZE; j++)
        {
            if (i < TILE_SIZE && j < TILE_SIZE)
                continue; // IGNORE START TILE
            int new_risk = space[i % TILE_SIZE][j % TILE_SIZE].risk + i / TILE_SIZE + j / TILE_SIZE;
            space[i][j].risk = new_risk < 10 ? new_risk : new_risk % 9;
            space[i][j].explored = false;
            space[i][j].min_risk_path = INT_MAX;
        }
    }
#endif

    space[0][0].min_risk_path = 0;
    bool modified;
    do
    {
        modified = false;
        for (size_t c = 0; c < SPACE_SIZE * 2; c++)
        {
            for (size_t x = 0; x <= c; x++)
            {
                int y = c - x;
                if (VALID_POSITION(x, y))
                {
                    space[x][y].explored = true;
                    node_risk(space, x + 1, y, space[x][y].min_risk_path, &modified);
                    node_risk(space, x, y + 1, space[x][y].min_risk_path, &modified);
                    node_risk(space, x - 1, y, space[x][y].min_risk_path, &modified);
                    node_risk(space, x, y - 1, space[x][y].min_risk_path, &modified);
                }
            }
        }
    } while (modified); // What the fuck? Brute forced it

    printf("Minimum risk %d\n", space[SPACE_SIZE - 1][SPACE_SIZE - 1].min_risk_path);

    return 0;
}

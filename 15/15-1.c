#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define PART_2

#define SPACE_SIZE 10

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
            // printf("%d(%c,%d)", space[i][j].risk, space[i][j].explored ? '*' : '.', space[i][j].min_risk_path);
            printf("%d(%d)", space[i][j].risk, space[i][j].min_risk_path);
        putchar('\n');
    }
    putchar('\n');
}

void node_risk(SPACE space, int x, int y, int risk)
{
    if (!VALID_POSITION(x, y))
        return;
    risk += space[x][y].risk;
    if (risk < space[x][y].min_risk_path)
        space[x][y].min_risk_path = risk;
}

int main(int argc, char const *argv[])
{
    FILE *p_file = fopen("input", "r");
    SPACE space = calloc(SPACE_SIZE, sizeof(CELL *));
    for (size_t i = 0; i < SPACE_SIZE; i++)
    {
        space[i] = calloc(SPACE_SIZE, sizeof(CELL));
        char buf[SPACE_SIZE + 2];
        fgets(buf, sizeof buf, p_file);
        for (size_t j = 0; j < SPACE_SIZE; j++)
        {
            space[i][j].risk = buf[j] - '0';
            space[i][j].explored = false;
            space[i][j].min_risk_path = __INT_MAX__;
        }
    }

    space[0][0].min_risk_path = 0;
    for (size_t i = 0; i < 500; i++) // What the fuck? Brute forced it
    {
        for (size_t c = 0; c < SPACE_SIZE * 2; c++)
        {
            for (size_t x = 0; x <= c; x++)
            {
                int y = c - x;
                if (VALID_POSITION(x, y))
                {
                    space[x][y].explored = true;
                    node_risk(space, x + 1, y, space[x][y].min_risk_path);
                    node_risk(space, x, y + 1, space[x][y].min_risk_path);
                    node_risk(space, x - 1, y, space[x][y].min_risk_path);
                    node_risk(space, x, y - 1, space[x][y].min_risk_path);
                }
            }
        }
    }
    // print_space(space);

    printf("Minimum risk %d\n", space[SPACE_SIZE - 1][SPACE_SIZE - 1].min_risk_path);

    return 0;
}

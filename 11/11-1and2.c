#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../check_alloc.h"

#define PART_2

typedef struct
{
    u_int8_t energy;
    bool lock; // LOCK STATE WHEN GLOWING
} OCTOPUS;

//

#define SPACE_SIZE 10

typedef OCTOPUS **SPACE;

#define VALID_VECTOR(i, j) ((i) >= 0 && (j) >= 0 && (i) < (SPACE_SIZE) && (j) < (SPACE_SIZE))
#define ITER_SPACE(i, j)                       \
    for (int i = 0; (i) < (SPACE_SIZE); (i)++) \
        for (int j = 0; (j) < (SPACE_SIZE); (j)++)
#define ITER_ADJACENT(i, j, x, y)                      \
    for (int x = ((i)-1); (x) <= ((i) + 1); (x)++)     \
        for (int y = ((j)-1); (y) <= ((j) + 1); (y)++) \
            if (((x) != (i) || (y) != (j)) && VALID_VECTOR((x), (y)))

bool check_sync(SPACE space)
{
    ITER_SPACE(i, j)
    {
        if (space[i][j].energy != 0)
            return false;
    }
    return true;
}

size_t flash(SPACE space)
{
    size_t n_flashes = 0;
    do // This can be further optimized with a update cache.
    {
        n_flashes = 0;
        ITER_SPACE(i, j)
        {
            if (space[i][j].energy > 9)
            {
                ITER_ADJACENT(i, j, x, y)
                {
                    OCTOPUS *p_cell = &space[x][y];
                    if (p_cell->energy > 9)
                    {
                        p_cell->lock = true;
                    }
                    else if (!p_cell->lock)
                    {
                        p_cell->energy++;
                        if (p_cell->energy > 9)
                            n_flashes++;
                    }
                }
                space[i][j].energy = 0;
                space[i][j].lock = true;
            }
        }
    } while (n_flashes > 0);

    ITER_SPACE(i, j)
    {
        if (space[i][j].lock)
        {
            n_flashes++;
            space[i][j].lock = false;
        }
    }
    return n_flashes;
}

void increment_energy(SPACE space)
{
    ITER_SPACE(i, j)
    {
        space[i][j].energy++;
    }
}

void print_space(SPACE space)
{
    for (size_t i = 0; i < SPACE_SIZE; i++)
    {
        for (size_t j = 0; j < SPACE_SIZE; j++)
        {
            if (space[i][j].energy)
                printf("%d ", space[i][j].energy);
            else
                printf("* ");
        }
        putchar('\n');
    }
    putchar('\n');
}

SPACE new_space(void)
{
    SPACE space = calloc(SPACE_SIZE, sizeof(OCTOPUS *));
    CHECK_ALLOC(space);
    for (size_t i = 0; i < SPACE_SIZE; i++)
    {
        space[i] = calloc(SPACE_SIZE, sizeof(OCTOPUS));
        CHECK_ALLOC(space[i]);
    }
    return space;
}

//

SPACE read_file(char *file)
{
    SPACE space = new_space();
    FILE *p_file = fopen(file, "r");
    CHECK_ALLOC(p_file);
    char buf[BUFSIZ];
    size_t j = 0;
    while (fgets(buf, sizeof buf, p_file) != NULL)
    {
        for (size_t i = 0; i < SPACE_SIZE; i++)
        {
            space[j][i].energy = buf[i] - '0';
        }
        j++;
    }
    return space;
}

int main(void)
{
    SPACE space = read_file("input");
    size_t total_flashes = 0;
#ifdef PART_2
    size_t step = 0;
    for (; !check_sync(space); step++)
#else
    for (size_t i = 0; i < 100; i++)
#endif
    {
        increment_energy(space);
        total_flashes += flash(space);
        // print_space(space);
    }
#ifdef PART_2
    printf("SYNC STEP %zu\n", step);
#else
    printf("FLASHES %zu\n", total_flashes);
#endif
}
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../check_alloc.h"

typedef struct
{
    bool **space; // space[y][x]
    int size_x;
    int size_y;
} GRID_SPACE;

GRID_SPACE *new_space()
{
    GRID_SPACE *p_space = calloc(1, sizeof(GRID_SPACE));
    CHECK_ALLOC(p_space);
    p_space->space = calloc(1, sizeof(bool *));
    CHECK_ALLOC(p_space->space);
    p_space->space[0] = calloc(1, sizeof(bool));
    CHECK_ALLOC(p_space->space[0]);
    p_space->size_x = 1;
    p_space->size_y = 1;
    return p_space;
}

void expand_x(GRID_SPACE *space, int new_x)
{
    new_x += 2;
    for (size_t i = 0; i < space->size_y; i++) //TODO:CHECK;
    {
        space->space[i] = realloc(space->space[i], new_x * sizeof(bool));
        for (size_t j = space->size_x; j < new_x; j++)
            space->space[i][j] = false; // RESET MEMORY
        CHECK_ALLOC(space->space[i]);
    }
    space->size_x = new_x;
}

void expand_y(GRID_SPACE *space, int new_y)
{
    new_y++;
    space->space = realloc(space->space, new_y * sizeof(bool *));
    CHECK_ALLOC(space->space);
    for (size_t i = space->size_y; i < new_y; i++)
    {
        space->space[i] = calloc(space->size_x, sizeof(bool));
        CHECK_ALLOC(space->space[i]);
    }
    space->size_y = new_y;
}

void resize_space(GRID_SPACE *space, int new_x, int new_y)
{
    if (new_x > space->size_x)
        expand_x(space, new_x);
    if (new_y > space->size_y)
        expand_y(space, new_y);
}

GRID_SPACE *read_space(FILE *p_file)
{
    int x, y;
    GRID_SPACE *p_space = new_space();
    CHECK_ALLOC(p_file);
    while (fscanf(p_file, "%d,%d\n", &x, &y) == 2)
    {
        resize_space(p_space, x, y);
        p_space->space[y][x] = true;
        // printf("%d %d\n", x, y);
    }
    // printf("Size %d %d\n", p_space->size_x, p_space->size_y);
    fscanf(p_file, "[^\n]\n"); // SKIP EMPTY LINE
    return p_space;
}

void print_space(GRID_SPACE *space)
{
    for (size_t i = 0; i < space->size_y; i++)
    {
        for (size_t j = 0; j < space->size_x; j++)
            putchar(space->space[i][j] ? '#' : '.');
        putchar('\n');
    }
    putchar('\n');
}

int count_visible(GRID_SPACE *space)
{
    int count = 0;
    for (size_t i = 0; i < space->size_y; i++)
        for (size_t j = 0; j < space->size_x; j++)
            if (space->space[i][j])
                count++;
    return count;
}

// fold
void fold_x(GRID_SPACE *space, int x)
{
    for (int i = 0; i < space->size_y; i++)
    {
        for (int j = x; j < space->size_x; j++)
            if (space->space[i][j])
                space->space[i][2 * x - j] = true;
    }
    space->size_x = x;
}

void fold_y(GRID_SPACE *space, int y)
{
    for (int i = y; i < space->size_y; i++)
    {
        for (int j = 0; j < space->size_x; j++)
            if (space->space[i][j])
                space->space[2 * y - i][j] = true;
    }
    space->size_y = y;
}

void fold(GRID_SPACE *space, FILE *p_file)
{
    char axis;
    int line;
    bool first = true;
    while (fscanf(p_file, "fold along %c=%d\n", &axis, &line) == 2)
    {
        if (axis == 'y')
            fold_y(space, line);
        else
            fold_x(space, line);
        if (first)
        {
            first = false;
            printf("FIRST VISIBLE: %d\n", count_visible(space));
        }
    };
}

//

int main(int argc, char const *argv[])
{
    FILE *p_file = fopen("input", "r");
    GRID_SPACE *p_space = read_space(p_file);
    fold(p_space, p_file);
    print_space(p_space);
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// PART 2 - DIAGONAL_LINES
#define PART_2

// SPACE

#define SPACE_SIZE 1000

typedef unsigned int **SPACE;

void print_space(SPACE space)
{
    for (size_t i = 0; i < SPACE_SIZE; i++)
    {
        for (size_t j = 0; j < SPACE_SIZE; j++)
        {
            if (space[j][i] == 0)
                putchar('.');
            else
                printf("%d", space[j][i]);
        }
        putchar('\n');
    }
    putchar('\n');
}

unsigned int count_intersections(SPACE space)
{
    unsigned int count = 0;
    for (size_t i = 0; i < SPACE_SIZE; i++)
    {
        for (size_t j = 0; j < SPACE_SIZE; j++)
        {
            if (space[j][i] > 1)
                count++;
        }
    }
    return count;
}

//
// LINE

typedef struct
{
    int x1;
    int y1;
    int x2;
    int y2;
} LINE;

void swap(unsigned int *v1, unsigned int *v2)
{
    unsigned int buf = *v1;
    *v1 = *v2;
    *v2 = buf;
}

// void print_line(LINE *line)
// {
//     printf("[%d,%d]->[%d,%d]\n", line->x1, line->y1, line->x2, line->y2);
// }

#ifndef PART_2
bool is_straight_line(LINE *line)
{
    return line->x1 == line->x2 || line->y1 == line->y2;
}
#endif

bool plot_line(LINE *line, SPACE space)
{
    int dx = line->x2 - line->x1;
#ifndef PART_2
    if (!is_straight_line(line)) //PART 1
        return false;
#endif
    int dy = line->y2 - line->y1;
    if (dx == 0 || abs(dy / dx) > 1)
    {
        int b = line->x1 - dx / dy * line->y1;
        if (dy < 0)
            swap(&line->y1, &line->y2);
        for (int y = line->y1; y <= line->y2; y++)
        {
            int x = dx / dy * y + b;
            space[x][y]++;
        }
    }
    else
    {
        int b = line->y1 - dy / dx * line->x1;
        if (dx < 0)
            swap(&line->x1, &line->x2);
        for (int x = line->x1; x <= line->x2; x++)
        {
            int y = dy / dx * x + b;
            space[x][y]++;
        }
    }
}

void read_space_file(FILE *file, SPACE space)
{
    char buf[BUFSIZ];
    LINE *line = calloc(1, sizeof(LINE));
    while (fscanf(file, "%d,%d -> %d,%d\n", &line->x1, &line->y1, &line->x2, &line->y2) == 4)
    {
        plot_line(line, space);
    }
}

int main(int argc, char const *argv[])
{
    FILE *p_file = fopen("input", "r");

    // MAKE SPACE
    SPACE space = calloc(SPACE_SIZE, sizeof(unsigned int *));
    for (size_t i = 0; i < SPACE_SIZE; i++)
    {
        space[i] = calloc(SPACE_SIZE, sizeof(unsigned int));
    }
    //

    read_space_file(p_file, space);
    print_space(space);
    printf("%d\n", count_intersections(space));
    return 0;
}

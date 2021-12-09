#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../check_alloc.h"

// BASIN_STATS DATATYPE

#define TOP_AREAS 3

typedef struct
{
    unsigned int risk;
    unsigned int top_areas;
} BASIN_STATS;

// GRID DATASTRUCTURE
typedef struct
{
    unsigned int height;
    bool scanned;
} CELL;
typedef struct
{
    CELL **grid;
    unsigned int height; // GRID DIMENSIONS
    unsigned int width;
} GRID;

GRID *new_grid(unsigned int height, unsigned int width)
{
    GRID *grid = calloc(1, sizeof(GRID));
    grid->grid = calloc(height, sizeof(CELL));
    grid->height = height;
    grid->width = width;
    return grid;
}

void print_grid(GRID *grid)
{
    for (size_t i = 0; i < grid->height; i++)
    {
        for (size_t j = 0; j < grid->width; j++)
        {
            printf("%d(%d)", grid->grid[i][j].height, grid->grid[i][j].scanned);
        }
        putchar('\n');
    }
    putchar('\n');
}

unsigned int get_value(GRID *grid, size_t y, size_t x)
{
    return (x < 0 || y < 0 || x >= grid->width || y >= grid->height || grid->grid[y][x].scanned)
               ? 10
               : grid->grid[y][x].height;
}

//

unsigned int get_basin_area(GRID *grid, size_t i, size_t j)
{
    unsigned int height = get_value(grid, i, j);
    unsigned int area = 1;
    grid->grid[i][j].scanned = true;
#define CHECK_ADJACENT(x, y, grid, area)            \
    if (get_value((grid), (x), (y)) < 9)            \
    {                                               \
        (area) += get_basin_area((grid), (x), (y)); \
    }

    CHECK_ADJACENT(i + 1, j, grid, area);
    CHECK_ADJACENT(i - 1, j, grid, area);
    CHECK_ADJACENT(i, j + 1, grid, area);
    CHECK_ADJACENT(i, j - 1, grid, area);
#undef CHECK_ADJACENT
    return area;
}

int sort_list(const void *x, const void *y)
{
    return *(int *)x - *(int *)y;
}

// PART 1
BASIN_STATS *get_risk(GRID *grid)
{
    BASIN_STATS *basin = calloc(1, sizeof(BASIN_STATS));
    unsigned int areas[TOP_AREAS + 1] = {0}; // TRACK 3 LARGEST AREAS
    unsigned int risk = 0;
    for (size_t i = 0; i < grid->height; i++)
    {
        for (size_t j = 0; j < grid->width; j++)
        {
            unsigned int height = get_value(grid, i, j);
            // MINIMUM
            if (get_value(grid, i + 1, j) > height && get_value(grid, i - 1, j) > height && get_value(grid, i, j + 1) > height && get_value(grid, i, j - 1) > height)
            {
                basin->risk += height + 1;
                areas[0] = get_basin_area(grid, i, j);
                qsort(areas, TOP_AREAS + 1, sizeof(areas[0]), sort_list);
            }
        }
    }
    basin->top_areas = 1;
    for (size_t i = 1; i < TOP_AREAS + 1; i++)
    {
        basin->top_areas *= areas[i];
    }
    return basin;
}

//

unsigned int get_lines(char *file)
{
    FILE *p_file = fopen(file, "r");
    unsigned int lines = 0;
    char c;
    while ((c = getc(p_file)) != EOF)
        if (c == '\n')
            lines++;
    return lines;
}

CELL *str_to_line(char *str, size_t len)
{
    CELL *line = calloc(len, sizeof(CELL));
    for (size_t i = 0; i < len; i++)
    {
        line[i].height = (str[i] - '0');
        line[i].scanned = false;
    }
    return line;
}

GRID *read_file(char *file)
{
    FILE *p_file = fopen(file, "r");
    unsigned int n_lines = get_lines(file);
    char buf[BUFSIZ];
    fgets(buf, sizeof buf, p_file);
    unsigned int width = strlen(buf) - 1;
    GRID *p_grid = new_grid(n_lines, width);
    size_t i = 0;
    do
    {
        p_grid->grid[i] = str_to_line(buf, width);
        i++;
    } while (fgets(buf, sizeof buf, p_file) != NULL);
    return p_grid;
}

int main(int argc, char const *argv[])
{
    GRID *p_grid = read_file("input");
    BASIN_STATS *p_basin = get_risk(p_grid);
    printf("RISK %d AREA %d\n", p_basin->risk, p_basin->top_areas);
    return 0;
}

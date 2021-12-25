#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef enum
{
    EMPTY = 0,
    EAST,
    SOUTH,
} DIRECTION;

typedef struct
{
    DIRECTION current_direction;
    DIRECTION next_direction;
} CELL;

typedef struct
{
    CELL **space;
    int width;
    int height;
    bool modified;
} SPACE;

SPACE *new_space(int height)
{
    SPACE *p_space = calloc(1, sizeof(SPACE));
    p_space->space = calloc(height, sizeof(CELL *));
    p_space->height = height;
    return p_space;
}

#define ITER_SPACE(space, i, j)                  \
    for (size_t i = 0; i < (space)->height; i++) \
        for (size_t j = 0; j < (space)->width; j++)

void step_space(SPACE *space)
{
    space->modified = false;
    ITER_SPACE(space, i, j)
    {
        if (space->space[i][j].current_direction == EAST &&
            space->space[i][(j + 1) % space->width].current_direction == EMPTY)
        {
            space->space[i][(j + 1) % space->width].next_direction = EAST;
            space->space[i][j].next_direction = EMPTY;
            space->modified = true;
        }
    }
    ITER_SPACE(space, i, j)
    {
        space->space[i][j].current_direction = space->space[i][j].next_direction;
    }
    ITER_SPACE(space, i, j)
    {
        if (space->space[i][j].current_direction == SOUTH &&
            space->space[(i + 1) % space->height][j].current_direction == EMPTY)
        {
            space->space[(i + 1) % space->height][j].next_direction = SOUTH;
            space->space[i][j].next_direction = EMPTY;
            space->modified = true;
        }
    }
    ITER_SPACE(space, i, j)
    {
        space->space[i][j].current_direction = space->space[i][j].next_direction;
    }
}

void *print_space(SPACE *space)
{
    for (size_t i = 0; i < space->height; i++)
    {
        for (size_t j = 0; j < space->width; j++)
        {
            switch (space->space[i][j].current_direction)
            {
            case EMPTY:
                putchar('.');
                break;
            case EAST:
                putchar('>');
                break;
            case SOUTH:
                putchar('v');
                break;
            }
        }
        putchar('\n');
    }
    putchar('\n');
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

SPACE *read_space_file(char *file_name)
{
    int lines = get_lines(file_name);
    SPACE *p_space = new_space(lines);
    FILE *p_file = fopen(file_name, "r");
    int i = 0;
    char buf[BUFSIZ];
    while (fgets(buf, sizeof buf, p_file) != NULL)
    {
        buf[strcspn(buf, "\n")] = '\0';
        int len = strlen(buf);
        p_space->space[i] = calloc(len, sizeof(CELL));
        p_space->width = len;
        for (int j = 0; j < len; j++)
        {
            DIRECTION direction;
            switch (buf[j])
            {
            case '.':
                direction = EMPTY;
                break;
            case '>':
                direction = EAST;
                break;
            case 'v':
                direction = SOUTH;
                break;
            }
            p_space->space[i][j].current_direction = direction;
            p_space->space[i][j].next_direction = direction;
        }
        i++;
    }
    return p_space;
}

int main(int argc, char const *argv[])
{
    SPACE *p_space = read_space_file("input");
    int step_counter = 0;
    do
    {
        step_space(p_space);
        step_counter++;
    } while (p_space->modified);

    // print_space(p_space);
    printf("STEP %d\n", step_counter);

    return 0;
}

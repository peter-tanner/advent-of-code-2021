#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../check_alloc.h"

#define LUT_LENGTH 512
#define ITERATIONS 50
// THERE'S SOME UNDEFINED BEHAVIOR WHICH
// EXPANDS FROM THE CORNERS - EXCLUDE THESE AREAS
#define EXCLUDED_AREA (ITERATIONS + 1)
#define EXPANDED_SPACE (ITERATIONS + EXCLUDED_AREA)

bool *get_lut(FILE *p_file)
{
    char buf[BUFSIZ];
    bool *lut = calloc(LUT_LENGTH, sizeof(bool));
    fgets(buf, sizeof buf, p_file);
    for (size_t i = 0; i < LUT_LENGTH; i++)
        lut[i] = buf[i] == '#';
    return lut;
}

typedef struct
{
    bool **space;
    int space_size;
} SPACE;

SPACE *get_space(FILE *p_file)
{
    SPACE *p_space = malloc(sizeof(SPACE));
    CHECK_ALLOC(p_space);
    char buf[BUFSIZ];
    fgets(buf, sizeof buf, p_file);
    if (strlen(buf) >= BUFSIZ)
        exit(EXIT_FAILURE);
    size_t ic_size = strlen(buf) - 1; // -1 FOR \n
    p_space->space_size = ic_size + 2 * (EXPANDED_SPACE);

    // ALLOCATE SPACE
    p_space->space = calloc(p_space->space_size, sizeof(bool *));
    CHECK_ALLOC(p_space->space);
    for (size_t i = 0; i < p_space->space_size; i++)
    {
        p_space->space[i] = calloc(p_space->space_size, sizeof(bool));
        CHECK_ALLOC(p_space->space[i]);
    }
    // SPACE

    for (size_t j = 0; j < ic_size; j++)
    {
        for (size_t i = 0; i < ic_size; i++)
            p_space->space[j + EXPANDED_SPACE][i + EXPANDED_SPACE] = buf[i] == '#';
        fgets(buf, sizeof buf, p_file);
    }
    return p_space;
}

void print_space(SPACE *space)
{
    for (size_t i = 0; i < space->space_size; i++)
    {
        for (size_t j = 0; j < space->space_size; j++)
            putchar(space->space[i][j] ? '#' : '.');
        putchar('\n');
    }
    putchar('\n');
}

bool space_value(SPACE *space, int i, int j)
{
    if (i < 0 || j < 0 || i >= space->space_size || j >= space->space_size)
        return false;
    return space->space[i][j];
}

//
void step_space(SPACE *space, bool *lut)
{
    bool buffer[space->space_size][space->space_size];
    memset(buffer, false, space->space_size * space->space_size * sizeof(bool));
    for (int i = 0; i < space->space_size; i++)
    {
        for (int j = 0; j < space->space_size; j++)
        {
            char number[9];
            int idx = 0;
            for (int y = j - 1; y <= j + 1; y++)
            {
                for (int x = i - 1; x <= i + 1; x++)
                {
                    number[idx] = space_value(space, y, x) ? '1' : '0';
                    idx++;
                }
            }
            buffer[j][i] = lut[strtol(number, NULL, 2)];
        }
    }
    for (int i = 0; i < space->space_size; i++)
        memcpy(space->space[i], buffer[i], space->space_size * sizeof(bool));
}

int count_space(SPACE *space)
{
    int count = 0;
    // IGNORE BOUNDARY (REPRESENTS INFINITE SIZE OF SPACE)
    for (size_t i = EXCLUDED_AREA; i < space->space_size - EXCLUDED_AREA; i++)
        for (size_t j = EXCLUDED_AREA; j < space->space_size - EXCLUDED_AREA; j++)
            count += space->space[i][j];
    return count;
}

int main(int argc, char const *argv[])
{
    FILE *p_file = fopen("input", "r");
    bool *lut = get_lut(p_file);

    fscanf(p_file, "%s", NULL); // SKIP LINE

    SPACE *space = get_space(p_file);

    for (size_t i = 0; i < ITERATIONS; i++)
        step_space(space, lut);
    printf("COUNT %d\n", count_space(space));

    return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../check_alloc.h"

typedef union
{
    struct _FISH_NUMBER *pair;
    int integer;
} ANY_NUMBER;

typedef struct _FISH_NUMBER
{
    ANY_NUMBER left;
    bool left_integer;
    ANY_NUMBER right;
    bool right_integer;
} FISH_NUMBER;

FISH_NUMBER *new_number()
{
    FISH_NUMBER *p_number = calloc(1, sizeof(FISH_NUMBER));
    CHECK_ALLOC(p_number);
    return p_number;
}

typedef enum
{
    FIRST,
    SECOND
} STATE;

FISH_NUMBER *str_2_number(char *number)
{
    FISH_NUMBER *p_number = new_number();
    number = strchr(number, '[');
    if ('[' == *(number + 1))
    {
        str_2_number(number + 1);
    }
}

int main(int argc, char const *argv[])
{
    char number[] = "[[1,2],3]";
    int a, b;
    printf("%d\n", sscanf(number, "%d", &a, &b));
    return 0;
}

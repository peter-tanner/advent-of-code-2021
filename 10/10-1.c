#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../check_alloc.h"

// Fun fact: I got really lucky on this one - for some reason my sorting wasn't
// working, but I guessed middle+1th index and got the right answer on my laptop
// In the end the issue is that I didn't use LIST_TYPE in the LINE_RESULT struct

#define OPENING_BRACKETS "([{<"

// LIST DATASTRUCTURE

#define INITIAL_SIZE 20
typedef size_t LIST_TYPE;

typedef struct
{
    LIST_TYPE *list;
    size_t size;
    size_t capacity;
} LIST;

LIST *new_list(void)
{
    LIST *p_list = calloc(1, sizeof(LIST));
    CHECK_ALLOC(p_list);
    p_list->capacity = INITIAL_SIZE;
    p_list->list = calloc(p_list->capacity, sizeof(LIST_TYPE));
    CHECK_ALLOC(p_list->list);
    p_list->size = 0;
    return p_list;
}

void add_list(LIST *list, LIST_TYPE elem)
{
    if (list->size >= list->capacity)
    {
        list->capacity *= 2;
        list->list = realloc(list->list, list->capacity * sizeof(LIST_TYPE));
        CHECK_ALLOC(list->list);
    }
    list->list[list->size] = elem;
    list->size++;
}

//

unsigned int get_error_points(char c)
{
    switch (c)
    {
    case ')':
        return 3;
    case ']':
        return 57;
    case '}':
        return 1197;
    case '>':
        return 25137;
    }
    return 0;
}

unsigned int get_autocomplete_points(char c)
{
    switch (c)
    {
    case ')':
        return 1;
    case ']':
        return 2;
    case '}':
        return 3;
    case '>':
        return 4;
    }
    return 0;
}

char closing_char(char opening_char)
{
    switch (opening_char)
    {
    case '(':
        return ')';
    case '[':
    case '{':
    case '<':
        return opening_char + 2;
    }
    return '\0';
}

enum MODE
{
    ERROR,
    AUTOCOMPLETE
};

typedef struct
{
    enum MODE mode;
    LIST_TYPE points;
} LINE_RESULT;

LINE_RESULT process_line(char *line)
{
    LINE_RESULT result;
    size_t len = strlen(line);

    // BRACKET STACK
    size_t stack_pointer = 0;
    char stack[len];

    for (size_t i = 0; i < len; i++)
    {
        if (line[i] == '\n')
            break;
        if (strchr(OPENING_BRACKETS, line[i]) != NULL)
        {
            stack[stack_pointer] = line[i];
            stack_pointer++;
        }
        else if (closing_char(stack[stack_pointer - 1]) == line[i])
        {
            stack_pointer--;
        }
        else
        {
            // printf("%s - Expected %c, but found %c instead\n",line,closing_char(stack[stack_pointer-1]),line[i]);
            result.mode = ERROR;
            result.points = get_error_points(line[i]);
            return result;
        }
    }

    // LINE IS INCOMPLETE
    result.mode = AUTOCOMPLETE;
    result.points = 0;
    while (stack_pointer > 0)
    {
        result.points *= 5;
        result.points += get_autocomplete_points(closing_char(stack[stack_pointer - 1]));
        stack_pointer--;
    }

    return result;
}

void read_file(char *file, unsigned int *err_points, LIST *autocomplete_points)
{
    FILE *p_file = fopen(file, "r");
    CHECK_ALLOC(p_file);
    char buf[BUFSIZ];
    int i = 0;
    while (fgets(buf, sizeof buf, p_file) != NULL)
    {
        LINE_RESULT line = process_line(buf);
        if (line.mode == ERROR)
            *err_points += line.points;
        else // AUTOCOMPLETE
            add_list(autocomplete_points, line.points);
    }
}

int compare_list(const void *a, const void *b)
{
    if (*(LIST_TYPE *)a == *(LIST_TYPE *)b)
        return 0;
    return *(LIST_TYPE *)a > *(LIST_TYPE *)b ? 1 : -1;
}

int main(int argc, char const *argv[])
{
    LIST *p_autocomplete_list = new_list();
    unsigned int err_points = 0;
    read_file("input", &err_points, p_autocomplete_list);
    qsort(p_autocomplete_list->list, p_autocomplete_list->size, sizeof(p_autocomplete_list->list[0]), compare_list);
    printf("ERROR POINTS %u, AUTOCOMPLETE POINTS %zu\n", err_points, p_autocomplete_list->list[p_autocomplete_list->size / 2]);
    return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../check_alloc.h"

#define INPUT_PATH "input"
#define WIDTH 12

// LIST DATASTRUCTURE
#define INITIAL_SIZE 20

typedef struct
{
    char **list;
    size_t size;
    size_t capacity;
} LIST;

void free_list(LIST *list)
{
    free(list->list);
    free(list);
}
void delete_list(LIST *list)
{
    for (size_t i = 0; i < list->size; i++)
    {
        free(list->list[i]);
    }
    free_list(list);
}

LIST *new_list(void)
{
    LIST *p_list = calloc(1, sizeof(LIST));
    p_list->capacity = INITIAL_SIZE;
    p_list->list = calloc(p_list->capacity, sizeof(char *));
    return p_list;
}

void add_list(LIST *list, char *elem)
{
    if (list->size >= list->capacity)
    {
        list->capacity *= 2;
        list->list = realloc(list->list, list->capacity * sizeof(char *));
        CHECK_ALLOC(list->list);
    }
    list->list[list->size] = elem;
    list->size++;
}

//

LIST *read_file(char *path)
{
    FILE *p_file = fopen(path, "r");
    LIST *p_list = new_list();
    char line[WIDTH + 2];
    while (fgets(line, sizeof line, p_file) != NULL)
    {
        add_list(p_list, strdup(line));
    }
    return p_list;
}

char common_value(LIST *list, char index)
{
    uint one = 0;
    uint zero = 0;
    for (size_t i = 0; i < list->size; i++)
    {
        if (list->list[i][index] - '0')
        {
            one++;
        }
        else
        {
            zero++;
        }
    }
    return one >= zero;
}

void partition(LIST *list, char index, bool co2, bool o2)
{
    if (index >= WIDTH || list->size == 1)
    {
        if (co2)
            printf("CO2\n");
        else
            printf("O2\n");
        printf("%lu\n", strtol(list->list[0], NULL, 2));
        return;
    }
    char common = common_value(list, index);
    LIST *p_list_co2 = new_list();
    LIST *p_list_o2 = new_list();
    for (size_t i = 0; i < list->size; i++)
    {
        if ((list->list[i][index] - '0') == common)
        {
            if (o2)
                add_list(p_list_o2, list->list[i]);
        }
        else
        {
            if (co2)
                add_list(p_list_co2, list->list[i]);
        }
    }
    if (o2)
        partition(p_list_o2, index + 1, false, true);
    if (co2)
        partition(p_list_co2, index + 1, true, false);
    free_list(p_list_o2);
    free_list(p_list_co2);
}

int main(int argc, char const *argv[])
{
    LIST *list = read_file(INPUT_PATH);
    partition(list, 0, true, true);
    delete_list(list);
    return 0;
}

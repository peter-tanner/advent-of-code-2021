#include <stdlib.h>
#include <stdio.h>
#include "../check_alloc.h"

#define CYCLE 6
#define STARTING_TIMER 8
unsigned int INITIAL_CONDITION[] = {3, 4, 3, 1, 2};

// LIST DATASTRUCTURE
#define INITIAL_SIZE 20

typedef struct
{
    unsigned int *list;
    size_t size;
    size_t capacity;
} LIST;

void free_list(LIST *list)
{
    free(list->list);
    free(list);
}

LIST *new_list(void)
{
    LIST *p_list = calloc(1, sizeof(LIST));
    p_list->capacity = INITIAL_SIZE;
    p_list->list = calloc(p_list->capacity, sizeof(unsigned int));
    return p_list;
}

void add_list(LIST *list, unsigned int elem)
{
    if (list->size >= list->capacity)
    {
        list->capacity *= 2;
        list->list = realloc(list->list, list->capacity * sizeof(unsigned int));
        CHECK_ALLOC(list->list);
    }
    list->list[list->size] = elem;
    list->size++;
}

void print_list(LIST *list)
{
    for (size_t i = 0; i < list->size; i++)
    {
        printf("%d ", list->list[i]);
    }
    putchar('\n');
}

//

void step_time(LIST *state)
{
    size_t size_before = state->size;
    for (size_t i = 0; i < size_before; i++)
    {
        if (state->list[i] == 0)
        {
            state->list[i] = CYCLE;
            add_list(state, STARTING_TIMER);
        }
        else
        {
            state->list[i]--;
        }
    }
}

int main(int argc, char const *argv[])
{
    LIST *state = new_list();
    size_t ic_size = sizeof(INITIAL_CONDITION) / sizeof(INITIAL_CONDITION[0]);
    for (size_t i = 0; i < ic_size; i++)
    {
        add_list(state, INITIAL_CONDITION[i]);
    }
    for (size_t i = 0; i < 80; i++)
    {
        step_time(state);
        printf("%lu\t", i);
        fflush(stdout);
        // print_list(state);
    }
    printf("\n%lu\n", state->size);
    return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include "../check_alloc.h"
#include <pthread.h>

// EXTENDING THE DUMB SOLUTION WITH THREADS - STILL BAD BECAUSE OF THE ALGORITHM'S BAD TIME COMPLEXITY.

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

void *manage_part(void *args)
{
    LIST *state = (LIST *)args;
    for (size_t i = 0; i < 256; i++)
    {
        step_time(state);
        printf("%lu\t", i);
        fflush(stdout);
        // print_list(state);
    }
}

int main(int argc, char const *argv[])
{
    size_t ic_size = sizeof(INITIAL_CONDITION) / sizeof(INITIAL_CONDITION[0]);
    LIST *parts[ic_size];
    pthread_t threads[ic_size];

    // MAKE THREADS AND STATE PARTS
    for (size_t i = 0; i < ic_size; i++)
    {
        parts[i] = new_list();
        add_list(parts[i], INITIAL_CONDITION[i]);
        pthread_create(&threads[i], NULL, manage_part, parts[i]);
    }

    size_t total = 0;
    for (size_t i = 0; i < ic_size; i++)
    {
        pthread_join(threads[i], NULL);
        total += parts[i]->size;
    }

    printf("\n%lu\n", total);
    return 0;
}

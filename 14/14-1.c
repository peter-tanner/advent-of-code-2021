#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../check_alloc.h"

// LIST(string) DATASTRUCTURE

#define INITIAL_SIZE BUFSIZ
typedef char LIST_TYPE;

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

LIST *read_template(FILE *p_file)
{
    LIST *p_string = new_list();
    char buf[BUFSIZ];
    fscanf(p_file, "%[^\n]\n\n", buf);
    strcpy(p_string->list, buf);
    return p_string;
}

int main(int argc, char const *argv[])
{
    FILE *p_file = fopen("input", "r");
    LIST *p_string = read_template(p_file);
    printf("%s\n", p_string->list);
    return 0;
}

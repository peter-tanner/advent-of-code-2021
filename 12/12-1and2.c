#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define PART_2

#define ADJACENT_NODE_SIZE 8 // MAX NUMBER OF CONNECTED NODES
#define MAX_CAVES 20
#define NAME_BUF 5

#ifdef PART_2
#define MAX_VISITS 2
#else
#define MAX_VISITS 1
#endif
// ASSUME STRING IS EITHER ALL UPPERCASE OR ALL LOWERCASE
size_t max_visits(char *name)
{
    if (strcmp(name, "start") == 0 || strcmp(name, "end") == 0)
        return 1;
    else if (isupper(name[0]) != 0)
        return 0;
    return MAX_VISITS; // LOWERCASE
}

typedef struct __node
{
    char *name;
    size_t max_visits; // 0 => INFINITE VISITS
    size_t visit_count;
    struct __node **linked;
    size_t id;
    int n_linked;
} NODE;

NODE *new_node(char *name, size_t id)
{
    NODE *p_node = calloc(1, sizeof(NODE));
    p_node->name = name;
    p_node->max_visits = max_visits(name);
    p_node->visit_count = 0;
    p_node->id = id;
    p_node->linked = calloc(ADJACENT_NODE_SIZE, sizeof(NODE *));
    p_node->n_linked = 0;
    return p_node;
}

void link_nodes(NODE *node1, NODE *node2)
{
    node2->linked[node2->n_linked] = node1;
    node1->linked[node1->n_linked] = node2;
    node1->n_linked++;
    node2->n_linked++;
}

// NOTE - A HASHTABLE WOULD WORK GREAT FOR VERY LARGE SYSTEMS, BUT SINCE
// THE CHALLENGE IS SMALL WE WILL USE A SIMPLE LIST.
typedef struct
{
    NODE **nodes;
    int n_nodes;
    NODE *start_node;
    NODE *end_node;
} CAVE_SYSTEM;

CAVE_SYSTEM *new_cave_system()
{
    CAVE_SYSTEM *p_cave_system = calloc(1, sizeof(CAVE_SYSTEM));
    p_cave_system->nodes = calloc(MAX_CAVES, sizeof(NODE *));
}

int add_new_cave(CAVE_SYSTEM *caves, char *name)
{
    if (caves->n_nodes > MAX_CAVES)
        exit(EXIT_FAILURE);
    for (int i = 0; i < caves->n_nodes; i++)
        if (strcmp(name, caves->nodes[i]->name) == 0)
            return i;
    caves->nodes[caves->n_nodes] = new_node(name, caves->n_nodes);
    if (caves->start_node == NULL && strcmp(name, "start") == 0)
        caves->start_node = caves->nodes[caves->n_nodes];
    else if (caves->end_node == NULL && strcmp(name, "end") == 0)
        caves->end_node = caves->nodes[caves->n_nodes];
    caves->n_nodes++;
    return caves->n_nodes - 1;
}

void print_links(CAVE_SYSTEM *caves)
{
    for (size_t i = 0; i < caves->n_nodes; i++)
    {
        printf("%s:\t", caves->nodes[i]->name);
        for (size_t j = 0; caves->nodes[i]->linked[j] != NULL; j++)
        {
            printf("%s ", caves->nodes[i]->linked[j]->name);
        }
        putchar('\n');
    }
}

// READ INPUT CAVES
void read_link(CAVE_SYSTEM *system, char *link_description)
{
    char buf1[NAME_BUF];
    int cave1_idx, cave2_idx;
    char buf2[NAME_BUF];
    sscanf(link_description, "%[^-]-%[^\n]\n", buf1, buf2);
    cave1_idx = add_new_cave(system, strdup(buf1));
    cave2_idx = add_new_cave(system, strdup(buf2));
    link_nodes(system->nodes[cave1_idx], system->nodes[cave2_idx]);
}

CAVE_SYSTEM *read_file(char *input)
{
    CAVE_SYSTEM *p_caves = new_cave_system();
    FILE *p_file = fopen(input, "r");
    char buf[BUFSIZ];
    while (fgets(buf, sizeof buf, p_file) != NULL)
        read_link(p_caves, buf);
    return p_caves;
}

//
// CAVE TRAVERSAL LOGIC
//

typedef struct
{
    NODE **excluded_nodes;
    NODE **path;
    int path_len;
    int n_paths;
#ifdef PART_2
    int n_double_visits;
#endif
} TRAVERSAL_CONTEXT;

TRAVERSAL_CONTEXT *new_traversal_context(void)
{
    TRAVERSAL_CONTEXT *p_context = calloc(1, sizeof(TRAVERSAL_CONTEXT));
    p_context->excluded_nodes = calloc(MAX_CAVES, sizeof(NODE *));
    p_context->path = calloc(MAX_VISITS * MAX_CAVES, sizeof(NODE *));
    p_context->path_len = 0;
    p_context->n_paths = 0;
#ifdef PART_2
    p_context->n_double_visits = 0;
#endif
    return p_context;
}

void print_path_stack(TRAVERSAL_CONTEXT *context)
{
    for (size_t i = 0; i < context->path_len; i++)
        printf("%s -> ", context->path[i]->name);
    putchar('\n');
}

bool excluded(TRAVERSAL_CONTEXT *context, NODE *node)
{
    for (size_t i = 0; i < MAX_CAVES; i++)
    {
        if (context->excluded_nodes != NULL && context->excluded_nodes[i] == node)
            return true;
    }
    return false;
}

void remove_excluded(TRAVERSAL_CONTEXT *context, NODE *node)
{
    for (size_t i = 0; i < MAX_CAVES; i++)
        if (context->excluded_nodes != NULL && context->excluded_nodes[i] == node)
        {
            context->excluded_nodes[i] = NULL;
#ifdef PART_2
            context->n_double_visits--;
#endif
        }
}
void add_excluded(TRAVERSAL_CONTEXT *context, NODE *node)
{
    for (size_t i = 0; i < MAX_CAVES; i++)
        if (context->excluded_nodes[i] != NULL)
        {
            if (context->excluded_nodes[i] == node)
                return;
        }
        else
        {
            context->excluded_nodes[i] = node;
#ifdef PART_2
            if (node->max_visits == MAX_VISITS)
                context->n_double_visits++;
#endif
            return;
        }
}

void finish_node(NODE *node, TRAVERSAL_CONTEXT *context)
{

    if (node->max_visits > 0)
    {
        node->visit_count--;
        if (node->visit_count < node->max_visits)
            remove_excluded(context, node);
    }
    context->path_len--;
}

void traverse(NODE *start, TRAVERSAL_CONTEXT *context)
{
    context->path[context->path_len] = start;
    context->path_len++;
    start->visit_count++;
#ifdef PART_2
    if (context->n_double_visits > 1 || excluded(context, start))
#else
    if (excluded(context, start))
#endif
    {
        finish_node(start, context);
        return;
    }
    if (strcmp(start->name, "end") == 0)
    {
        // printf("%s : ", start->name);
        // print_path_stack(context);
        context->n_paths++;
        finish_node(start, context);
        return;
    }

    if (start->max_visits > 0 && start->visit_count >= start->max_visits)
        add_excluded(context, start);
    for (size_t i = 0; start->linked[i] != NULL; i++)
        traverse(start->linked[i], context);
    finish_node(start, context);
}

int main(int argc, char const *argv[])
{
    CAVE_SYSTEM *p_caves = read_file("input");
    print_links(p_caves);
    TRAVERSAL_CONTEXT *p_context = new_traversal_context();
    traverse(p_caves->start_node, p_context);
    printf("n_paths : %d\n", p_context->n_paths);
    return 0;
}

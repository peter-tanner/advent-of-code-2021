#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// RELATIONSHIP datatype

#define ENTRIES 200
#define INPUT_COUNT 10
#define OUTPUT_COUNT 4
#define CABLES 7

typedef struct
{
    char **input;
    char **output;
} RELATIONSHIP;

RELATIONSHIP *new_relationship()
{
    RELATIONSHIP *p_relationship = calloc(1, sizeof(RELATIONSHIP));
    p_relationship->input = calloc(INPUT_COUNT, sizeof(char *));
    p_relationship->output = calloc(OUTPUT_COUNT, sizeof(char *));
    for (size_t i = 0; i < INPUT_COUNT; i++)
    {
        p_relationship->input[i] = calloc(CABLES, sizeof(char));
    }
    for (size_t i = 0; i < OUTPUT_COUNT; i++)
    {
        p_relationship->output[i] = calloc(CABLES, sizeof(char));
    }
    return p_relationship;
}

RELATIONSHIP *string_2_relationship(char *string)
{
    RELATIONSHIP *p_relationship = new_relationship();
    char input_buf[BUFSIZ];
    char output_buf[BUFSIZ];
    sscanf(string, "%[^|] | %[^\n]\n", input_buf, output_buf);
    char *wire = strtok(input_buf, " ");
    for (size_t i = 0; wire != NULL; i++)
    {
        p_relationship->input[i] = strdup(wire);
        wire = strtok(NULL, " ");
    }
    wire = strtok(output_buf, " ");
    for (size_t i = 0; wire != NULL; i++)
    {
        p_relationship->output[i] = strdup(wire);
        wire = strtok(NULL, " ");
    }
    return p_relationship;
}

unsigned int filter_trivial(RELATIONSHIP *relationship)
{
    unsigned int count = 0;
    for (size_t i = 0; relationship->output[i] != NULL; i++)
    {
        switch (strlen(relationship->output[i]))
        {
        case 2:
        case 3:
        case 4:
        case 7:
            count++;
            break;
        }
    }
    return count;
}

//

RELATIONSHIP **read_input(FILE *p_file)
{
    char buffer[BUFSIZ];
    RELATIONSHIP **p_wire_relationships = calloc(ENTRIES, sizeof(RELATIONSHIP *));

    for (size_t i = 0; fgets(buffer, sizeof buffer, p_file) > 0; i++)
    {
        p_wire_relationships[i] = string_2_relationship(buffer);
    }
    return p_wire_relationships;
}

int main(int argc, char const *argv[])
{
    FILE *p_file = fopen("input", "r");
    RELATIONSHIP **p_wire_relationships = read_input(p_file);

    unsigned int trivial_count = 0;
    for (size_t i = 0; p_wire_relationships[i] != NULL; i++)
    {
        trivial_count += filter_trivial(p_wire_relationships[i]);
    }
    printf("%d\n", trivial_count);
    return 0;
}

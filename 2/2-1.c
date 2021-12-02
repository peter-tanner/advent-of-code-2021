#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    FILE *p_file = fopen("input", "r");
    char instruction[50];

    int horizontal, depth = 0;

    while (fgets(instruction, sizeof instruction, p_file) != NULL)
    {
        char direction[50];
        int *amount = malloc(sizeof(int));
        if (sscanf(instruction, "%s %i", direction, amount) != 2)
            exit(EXIT_FAILURE);
        if (strcmp(direction, "up") == 0)
            (*amount) *= -1;
        if (strcmp(direction, "forward") == 0)
        {
            horizontal += *amount;
        }
        else
        {
            depth += *amount;
        }
        // printf("POSITION horizontal %d, depth %d\n", horizontal, depth);
    }
    printf("%d\n", depth * horizontal);
    return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    FILE *p_file = fopen("input", "r");
    char instruction[50];

    int horizontal, depth, aim = 0;

    while (fgets(instruction, sizeof instruction, p_file) != NULL)
    {
        char direction[50];
        int *amount = malloc(sizeof(int));
        if (sscanf(instruction, "%s %i", direction, amount) != 2)
            exit(EXIT_FAILURE);
        if (strcmp(direction, "up") == 0)
        {
            aim -= *amount;
        }
        else if (strcmp(direction, "down") == 0)
        {
            aim += *amount;
        }
        else // FORWARD
        {
            horizontal += *amount;
            depth += (*amount) * aim;
        }
        // printf("POSITION horizontal %d\t depth %d\t aim %d\n",
        //        horizontal, depth, aim);
    }
    printf("%d\n", depth * horizontal);
    return 0;
}

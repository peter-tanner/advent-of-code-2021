#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

int main(int argc, char const *argv[])
{
    FILE *p_file = fopen("input", "r");
    char line[50];
    unsigned int increases = 0;
    unsigned int previous_depth = 0;
    while (fgets(line, sizeof line, p_file) != NULL)
    {
        if (atoi(line) > previous_depth)
        {
            increases++;
        }
        previous_depth = atoi(line);
    }
    printf("%u\n", increases - 1);
    return 0;
}

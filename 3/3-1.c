#include <stdlib.h>
#include <stdio.h>

#define WIDTH 5

int main(int argc, char const *argv[])
{
    FILE *p_file = fopen("input", "r");

    unsigned int n_one[WIDTH] = {0};
    unsigned int length = 0;

    char line[WIDTH + 2];
    while (fgets(line, sizeof line, p_file) != NULL)
    {
        length++;
        for (size_t i = 0; i < WIDTH; i++)
        {
            if (line[i] - '0') // 1
            {
                n_one[i]++;
            }
        }
    }

    // GET GAMMA/EPSILON
    unsigned int gamma = 0, epsilon = 0;
    unsigned int place = 1;
    for (size_t i = WIDTH; i--;)
    {
        if (n_one[i] > length / 2)
        {
            gamma += place;
        }
        else
        {
            epsilon += place;
        }
        place *= 2;
    }

    printf("%d\n", gamma * epsilon);

    return 0;
}

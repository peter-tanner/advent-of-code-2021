#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// IC
#define TIMESPAN_DAYS 256
unsigned long INITIAL_CONDITION[] = {3, 4, 3, 1, 2};

int main(int argc, char const *argv[])
{
    // IC LOGIC
    bool is_ic_done = false;
    size_t ic_done_count = 0;
    size_t ic_size = sizeof(INITIAL_CONDITION) / sizeof(INITIAL_CONDITION[0]);

    // Had to scratch this one out on paper to figure it out.
    unsigned long cycles7[7] = {0};
    unsigned long cycles9[9] = {0};
    unsigned long total = 0;

    for (size_t t = 0; t < TIMESPAN_DAYS; t++)
    {
        // APPLY IC BY ADDING TO CYCLES AT OFFSETS SPECIFIED IN ICS
        if (!is_ic_done)
        {
            for (size_t i = 0; i < ic_size; i++)
            {
                if (t == INITIAL_CONDITION[i])
                {
                    cycles9[t % 9]++;
                    total++;
                    ic_done_count++;
                }
            }
            if (ic_done_count > ic_size)
                is_ic_done = true;
        }

        // UPDATE POPULATION
        unsigned long v = cycles7[t % 7];
        total += cycles9[t % 9];
        total += v;
        cycles7[t % 7] += cycles9[t % 9];
        cycles9[t % 9] += v;
    }
    printf("%lu\n", total);
    return 0;
}

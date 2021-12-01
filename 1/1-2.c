#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

#define N_WINDOWS 4
#define WINDOW_LENGTH 3

typedef struct
{
    uint sum;
    uint count;
    bool written; // IGNORE COMPARISONS AT THE START.
} MEASUREMENT_WINDOW;

int main(int argc, char const *argv[])
{
    FILE *p_file = fopen("input", "r");
    char line[50];
    uint increases = 0;

    MEASUREMENT_WINDOW windows[N_WINDOWS];

    uint i = 0;
    while (fgets(line, sizeof line, p_file) != NULL)
    {
        uint n = atoi(line);
        // UPDATE MEASURING WINDOWS
        for (size_t j = 0; j < N_WINDOWS; j++)
        {
            MEASUREMENT_WINDOW *window = &windows[j];
            if ((i - j) % N_WINDOWS == 0)
            {
                window->count = 0;
                window->sum = 0;
                window->written = true;
            }
            if (window->written && window->count < 3)
            {
                window->sum += n;
                window->count++;
            }
        }
        // COMPARE WINDOWS
        for (size_t j = 0; j < N_WINDOWS; j++)
        {
            MEASUREMENT_WINDOW *window = &windows[j];
            // ONE WINDOW IS READY TO BE COMPARED
            if (window->count == 3)
            {
                window->count++; // PREVENT DOUBLE-COMPARE OF WINDOW.
                MEASUREMENT_WINDOW last_window = windows[(j + N_WINDOWS - 1) % 4];
                if (last_window.written && window->written && window->sum > last_window.sum)
                {
                    increases++;
                }
                continue;
            }
        }
        i++;
    }
    printf("%u\n", increases);
    return 0;
}

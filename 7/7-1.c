#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

// SOLUTION USING GRADIENT DESCENT

#define PART_2

#ifdef PART_2
#define STEP 0.0001F
#else
#define STEP 0.2F
#endif

#define INITIAL_CONDITION              \
    {                                  \
        16, 1, 2, 0, 4, 2, 7, 1, 2, 14 \
    }
#define INITIAL_GUESS 10000.0F

// THIS IS NEEDED FOR PART 2 BECAUSE OF FLOATS. QUICK AND DIRTY COPYPASTE.
int fuel_total_int(int *state, size_t state_len, int point)
{
    int total = 0;
    for (size_t i = 0; i < state_len; i++)
    {
        int fuel = abs(state[i] - point);
#ifdef PART_2
        fuel *= (fuel + 1) / 2.0F; // TRIANGLE NUMBER T_n = n/2*(n+1)
#endif
        total += fuel;
    }
    return total;
}

// MUST BE FLOAT FOR GRADIENT DESCENT TO WORK.
float fuel_total(int *state, size_t state_len, float point)
{
    float total = 0;
    for (size_t i = 0; i < state_len; i++)
    {
        float fuel = fabs(state[i] - point);
#ifdef PART_2
        fuel *= (fuel + 1) / 2.0F; // TRIANGLE NUMBER T_n = n/2*(n+1)
#endif
        total += fuel;
    }
    return total;
}

void gradient_descent(int *state, size_t state_len, float *guess, float *gradient)
{
    float y = fuel_total(state, state_len, *guess);
    float dy_dx = fuel_total(state, state_len, *guess + 1) - y;
    printf("%.5f, y=%.5f, dy=%.5f\n", *guess, y, dy_dx);
    *guess -= STEP * dy_dx;
    *gradient = dy_dx;
}

float main(float argc, char const *argv[])
{
    int state[] = INITIAL_CONDITION;
    size_t state_len = sizeof(state) / sizeof(state[0]);
    float guess = INITIAL_GUESS;
    float gradient = 1.0F; // START WITH NON-ZERO VALUE
    while (fabs(gradient) > 0.01)
    {
        gradient_descent(state, state_len, &guess, &gradient);
    }

    // CHECK "AREA" SURROUNDING FINAL GUESS
    int min_fuel = __INT_MAX__;
    for (int t = guess - 1; t < guess + 1; t++)
    {
        int fuel = fuel_total_int(state, state_len, t);
        if (fuel < min_fuel)
            min_fuel = fuel;
    }

    printf("FUEL %d\n", (int)min_fuel);

    return 0;
}

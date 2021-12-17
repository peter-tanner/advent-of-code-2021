#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define X_MIN 20
#define X_MAX 30
#define Y_MIN -10
#define Y_MAX -5

#define FACTOR 2 // Factor determines search domain

typedef struct
{
    int x;
    int y;
} VECTOR2;

bool in_target(VECTOR2 vector)
{
    return vector.x >= X_MIN && vector.x <= X_MAX && vector.y >= Y_MIN && vector.y <= Y_MAX;
}
bool past_target(VECTOR2 vector)
{
    return vector.x > X_MAX || vector.y < Y_MIN;
}

VECTOR2 add_vectors(VECTOR2 v1, VECTOR2 v2)
{
    VECTOR2 new = {v1.x + v2.x, v1.y + v2.y};
    return new;
}

void print_vector(VECTOR2 v)
{
    printf("[%d,%d]\n", v.x, v.y);
}

bool try_velocity(VECTOR2 velocity, int *max_height)
{
    VECTOR2 position = {0, 0};
    int max_height_l = 0;
    while (true)
    {
        position = add_vectors(position, velocity);
        if (position.y > max_height_l)
            max_height_l = position.y;
        // print_vector(position);
        // UPDATE VELOCITY
        velocity.y--;
        velocity.x -= (velocity.x > 0 ? 1 : 0); // DRAG
        if (in_target(position))
        {
            if (max_height_l > *max_height)
                *max_height = max_height_l; // Required because FACTOR > 1
            return true;
        }
        if (past_target(position))
            return false;
    }
}

int main(int argc, char const *argv[])
{
    // Really naive solution which iterates over many potential velocities,
    // because the search domain is really small.
    int max_height = 0;
    int valid_iv = 0;
    for (int i = 0; i < X_MAX * FACTOR; i++)
    {
        for (int j = -abs(Y_MIN * FACTOR); j < abs(Y_MIN * FACTOR); j++)
        {
            VECTOR2 velocity = {i, j};
            if (try_velocity(velocity, &max_height))
                valid_iv++;
        }
    }
    printf("MAX_HEIGHT %d N_VALID_IV %d\n", max_height, valid_iv);
    return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define SPACE_RANGE 50
#define RANGE_BUF 512

typedef struct
{
    int x;
    int y;
} VECTOR2;

typedef struct
{
    int x;
    int y;
    int z;
} VECTOR3;

typedef struct
{
    int start;
    int end;
} RANGE2;

typedef struct
{
    RANGE2 xrange;
    RANGE2 yrange;
    RANGE2 zrange;
} RANGE3;

typedef enum
{
    ON,
    OFF
} CUBE_STATE;

typedef struct
{
    RANGE3 range;
    CUBE_STATE state;
} CUBE_RANGE3;

RANGE3 _SPACE_RANGE = {{-SPACE_RANGE, SPACE_RANGE}, {-SPACE_RANGE, SPACE_RANGE}, {-SPACE_RANGE, SPACE_RANGE}};

#define RANGE2_IN_RANGE2(first, second) ((first).start >= (second).start && (second).end >= (first).end)

#define RANGE3_IN_RANGE3(first, second) (RANGE2_IN_RANGE2((first).xrange, (second).xrange) && \
                                         RANGE2_IN_RANGE2((first).yrange, (second).yrange) && \
                                         RANGE2_IN_RANGE2((first).zrange, (second).zrange))

#define VECTOR3_IN_RANGE3(vector, range) ((vector).x >= (range).xrange.start && (vector).x <= (range).xrange.end && \
                                          (vector).y >= (range).yrange.start && (vector).y <= (range).yrange.end && \
                                          (vector).z >= (range).zrange.start && (vector).z <= (range).zrange.end)

#define RANGE3_VOLUME(range) (((range).xrange.end - (range).xrange.start) * \
                              ((range).yrange.end - (range).yrange.start) * \
                              ((range).zrange.end - (range).zrange.start))

#define ITER_RANGE3(range, vx, vy, vz)                                          \
    for (int vx = (range).xrange.start; (vx) <= (range).xrange.end; (vx)++)     \
        for (int vy = (range).yrange.start; (vy) <= (range).yrange.end; (vy)++) \
            for (int vz = (range).zrange.start; (vz) <= (range).zrange.end; (vz)++)

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))
RANGE3 intersection(RANGE3 range1, RANGE3 range2)
{
    RANGE3 intersection_range = {
        {MAX(range1.xrange.start, range2.xrange.start), MIN(range1.xrange.end, range2.xrange.end)},
        {MAX(range1.yrange.start, range2.yrange.start), MIN(range1.yrange.end, range2.yrange.end)},
        {MAX(range1.zrange.start, range2.zrange.start), MIN(range1.zrange.end, range2.zrange.end)},
    };
    return intersection_range;
}

int scan_range(FILE *p_file, char *state, RANGE3 *range)
{
    return fscanf(p_file, "%s x=%d..%d,y=%d..%d,z=%d..%d\n",
                  state, &range->xrange.start, &range->xrange.end, &range->yrange.start, &range->yrange.end, &range->zrange.start, &range->zrange.end);
}

int main(int argc, char const *argv[])
{
    FILE *p_file = fopen("input", "r");
    char buf[BUFSIZ];
    RANGE3 range;
    CUBE_RANGE3 ranges[RANGE_BUF];
    int ranges_size = 0;
    while (scan_range(p_file, buf, &range) == 7)
    {
        if (RANGE3_IN_RANGE3(range, _SPACE_RANGE))
        {
            ranges[ranges_size].range = range;
            ranges[ranges_size].state = strcmp(buf, "on") == 0 ? ON : OFF;
            ranges_size++;
        }
    }

    int on_count = 0;
    for (size_t i = 0; i < ranges_size; i++)
    {
        // if (ranges[i].state == OFF)
        //     continue;
        ITER_RANGE3(ranges[i].range, x, y, z)
        {
            VECTOR3 point = {x, y, z};
            bool covered = false;
            CUBE_STATE previous_state = OFF;
            for (size_t j = 0; j < i; j++)
            {
                if (VECTOR3_IN_RANGE3(point, ranges[j].range))
                {
                    covered = true;
                    previous_state = ranges[j].state;
                }
            }
            if (!covered && ranges[i].state == ON || previous_state != ranges[i].state)
            {
                on_count += ranges[i].state == OFF ? -1 : 1;
            }
        }
        RANGE3 range = ranges[i].range;
        printf("STATE %s, %d..%d %d..%d %d..%d\n",
               ranges[i].state == ON ? "ON" : "OFF", range.xrange.start, range.xrange.end, range.yrange.start, range.yrange.end, range.zrange.start, range.zrange.end);
        printf("%d\n", on_count);
    }

    return 0;
}

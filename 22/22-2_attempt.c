#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// ATTEMPT USING CUBE SPLITTING.

#define PART_2

#define SPACE_RANGE 50
#define RANGE_BUF 512
#define RANGE_SET_MAX 512

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

// Stack won't work well because an OFF range must split ON ranges that came before it
typedef struct
{
    CUBE_RANGE3 *ranges;
    int size;
} CUBE_RANGE3_SET;

CUBE_RANGE3_SET new_rangeset()
{
    CUBE_RANGE3_SET set;
    set.ranges = calloc(RANGE_SET_MAX, sizeof(CUBE_RANGE3));
    set.size = 0;
    return set;
}

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

int scan_range(FILE *p_file, char *state, RANGE3 *range)
{
    return fscanf(p_file, "%s x=%d..%d,y=%d..%d,z=%d..%d\n",
                  state, &range->xrange.start, &range->xrange.end, &range->yrange.start, &range->yrange.end, &range->zrange.start, &range->zrange.end);
}

int main(int argc, char const *argv[])
{
    FILE *p_file = fopen("input", "r");
    char buf[BUFSIZ];
    RANGE3 range3_buf;
    CUBE_RANGE3_SET ranges[RANGE_BUF];
    for (size_t i = 0; i < RANGE_BUF; i++)
        ranges[i] = new_rangeset();
    int ranges_size = 0;
    while (scan_range(p_file, buf, &range3_buf) == 7)
    {
        CUBE_RANGE3 range = {
            range3_buf,
            strcmp(buf, "on") == 0 ? ON : OFF};
        // #ifndef PART_2
        if (RANGE3_IN_RANGE3(range.range, _SPACE_RANGE))
        // #endif
        {
            ranges[ranges_size].ranges[0] = range;
            ranges[ranges_size].size++;
            ranges_size++;

            // ITER_RANGE3(range.range, x, y, z)
            // {
            //     VECTOR3 point = {x, y, z};
            //     bool covered = false;
            //     CUBE_STATE previous_state = OFF;
            //     for (size_t j = 0; j < ranges_size - 1; j++)
            //     {
            //         if (VECTOR3_IN_RANGE3(point, ranges[j].range))
            //         {
            //             covered = true;
            //             previous_state = ranges[j].state;
            //         }
            //     }
            //     if (!covered && range.state == ON || previous_state != range.state)
            //     {
            //         on_count += range.state == OFF ? -1 : 1;
            //     }
            // }
        }
    }

    for (size_t i = 0; i < ranges_size; i++)
    {
        // KEEP SPLITTING SUB-RANGES IF NECESSARY
        for (size_t j = 0; j < ranges[i].size; j++)
        {
            // ITERATE OVER PREVIOUS RANGES
            for (size_t k = 0; k < i; k++)
            {
                // AND OVER PREVIOUS SUB-RANGES
                for (size_t w = 0; w < ranges[k].size; w++)
                {
                    if (ranges[k].ranges[w].state != ranges[i].ranges[j].state && RANGE3_IN_RANGE3(ranges[k].ranges[w].range, ranges[i].ranges[j].range))
                    {
                        RANGE3 tmp_range = ranges[i].ranges[j].range;
                        RANGE3 sub_range = ranges[k].ranges[w].range;
                        CUBE_STATE state = ranges[i].ranges[j].state;

                        CUBE_RANGE3 buf_A = {{tmp_range.xrange, {tmp_range.yrange.start, sub_range.yrange.start}, sub_range.zrange}, state};                                 // A
                        CUBE_RANGE3 buf_B = {{tmp_range.xrange, {sub_range.yrange.end, tmp_range.yrange.end}, sub_range.zrange}, state};                                     // B
                        CUBE_RANGE3 buf_C = {{{tmp_range.xrange.start, sub_range.xrange.start}, {sub_range.yrange.start, sub_range.yrange.start}, sub_range.zrange}, state}; // C
                        CUBE_RANGE3 buf_D = {{{sub_range.xrange.end, tmp_range.xrange.end}, {sub_range.yrange.end, sub_range.yrange.end}, sub_range.zrange}, state};         // D
                        CUBE_RANGE3 buf_BOTTOM = {{tmp_range.xrange, tmp_range.yrange, {tmp_range.zrange.start, sub_range.zrange.start}}, state};                            // BOTTOM
                        CUBE_RANGE3 buf_TOP = {{tmp_range.xrange, tmp_range.yrange, {sub_range.zrange.end, tmp_range.zrange.end}}, state};                                   // TOP

                        ranges[i].ranges[j] = buf_A;
                        ranges[i].ranges[ranges[i].size] = buf_B;
                        ranges[i].size++;
                        ranges[i].ranges[ranges[i].size] = buf_C;
                        ranges[i].size++;
                        ranges[i].ranges[ranges[i].size] = buf_D;
                        ranges[i].size++;
                        ranges[i].ranges[ranges[i].size] = buf_BOTTOM;
                        ranges[i].size++;
                        ranges[i].ranges[ranges[i].size] = buf_TOP;
                        ranges[i].size++;
                    }
                }
            }
        }
    }

    int vol = 0;
    for (size_t i = 0; i < ranges_size; i++)
    {
        for (size_t j = 0; j < ranges[i].size; j++)
        {
            vol += RANGE3_VOLUME(ranges[i].ranges[j].range);
        }
    }
    printf("%d\n", vol);

    // for (size_t i = 0; i < ranges_size; i++)
    // {
    //     // if (ranges[i].state == OFF)
    //     //     continue;
    //     RANGE3 range = ranges[i].range;
    //     printf("STATE %s, %d..%d %d..%d %d..%d\n",
    //            ranges[i].state == ON ? "ON" : "OFF", range.xrange.start, range.xrange.end, range.yrange.start, range.yrange.end, range.zrange.start, range.zrange.end);
    // }
    return 0;
}

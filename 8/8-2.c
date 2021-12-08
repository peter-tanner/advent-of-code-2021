/*
* HIGHLY OVERCOMPLICATED SOLUTION USING BOOLEAN LOGIC
* THE MAPPING LOGIC WAS SKETCHED OUT ON PAPER :
* WE KNOW THAT 1 HAS TWO SEGMENTS ON THE RIGHT, AND 7 IS A ONE WITH AN EXTRA TOP 
SEGMENT. SO WE KNOW THE VALUE OF THIS TOP SEGMENT.
* COMBINING 7 AND 4 RESULTS IN A SHAPE SIMILAR TO 9 (MISSING THE BOTTOM SEGMENT)
SEARCH THROUGH THE LIST FOR SOMETHING WITH 6 SEGMENTS ACTIVE AND ONLY ONE
DIFFERENCE FROM THE UNION OF 7 & 4. THIS IS THE BOTTOM SEGMENT.
* COMBINE THE TOP AND BOTTOM SEGMENTS AND ONE. DO THE SAME PREVIOUS PROCESS BUT
WITH THIS NEW UNION AND THE NUMBER 3. WE NOW KNOW THE MIDDLE SEGMENT
* COMBINE ONE AND TOP, MIDDLE AND BOTTOM SEGMENTS. USE THE 9 WE FOUND EARLIER
TO FIND THE TOP LEFT SEGMENT
* COMBINE TOP, MIDDLE, BOTTOM, LEFT TOP SEGMENTS AND FIND 5 TO GET THE BOTTOM
RIGHT SEGMENT
* USING 1 AND THE BOTTOM RIGHT SEGMENT GET THE TOP RIGHT SEGMENT
* USING 8 AND ALL THE SEGMENTS WE'VE COLLECTED, FIND THE BOTTOM LEFT SEGMENT
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// STRUCTURE OF INPUT
#define ENTRIES 200
#define DIGIT_LEN 10
#define SEGMENT_GROUP_LEN 4

// RELATIONSHIP DEFINES
#define INPUT_COUNT 10
#define OUTPUT_COUNT 4
#define CABLES 7

#define CHAR_BIN(char) (1 << ((char)-'a'))

typedef struct
{
    char top;
    char right_top;
    char right_bottom;
    char left_top;
    char left_bottom;
    char bottom;
    char middle;
} SEVEN_SEGMENT;

typedef struct
{
    u_int8_t value;
    u_int8_t enabled;
} DIGIT;

DIGIT *new_digit(u_int8_t value)
{
    DIGIT *p_digit = calloc(1, sizeof(DIGIT));
    p_digit->value = value;
    return p_digit;
}

void set_digit(DIGIT *digit, char position, bool state)
{
    if (state)
    {
        digit->value |= CHAR_BIN(position);
        digit->enabled++;
    }
    else
    {
        digit->value &= ~(CHAR_BIN(position));
        digit->enabled--;
    }
}

DIGIT str_to_digit(char *digit_str)
{
    DIGIT digit = {0};
    size_t len = strlen(digit_str);
    for (size_t i = 0; i < len; i++)
    {
        set_digit(&digit, digit_str[i], true);
    }
    return digit;
}

u_int8_t get_digit_value(DIGIT *mappings[], DIGIT digit)
{
    for (size_t i = 0; i < DIGIT_LEN; i++)
    {
        if (digit.value == mappings[i]->value)
            return i;
    }
    exit(EXIT_FAILURE);
}

char position_value(u_int8_t position_bin)
{
    char result = 'a';
    while (position_bin >>= 1)
        result++;
    return result;
}

// TRUE IF NUMBER CONTAINS ALL DIGITS IN CONTAINED
bool contains(u_int8_t number, u_int8_t contained)
{
    for (size_t i = 0; i < 8; i++)
    {
        if ((contained >> i) & 1 && !((number >> i) & 1))
        {
            return false;
        }
    }
    return true;
}

// ---
// DO MAPPINGS

void set_digit_mapping(char **input, DIGIT **digit_mapping)
{
    SEVEN_SEGMENT seven_segment;
    DIGIT *unknown_digits = calloc(DIGIT_LEN, sizeof(DIGIT)); // This will result in a memory leak on big datasets, because we cannot free it easily.
    // DECODE WIRES TO SEGMENTS
    for (size_t i = 0; i < DIGIT_LEN; i++)
    {
        unknown_digits[i] = str_to_digit(input[i]);
        // TRIVIAL, UNIQUE LENGTH CASES
        switch (unknown_digits[i].enabled)
        {
        case 7: // EIGHT
            digit_mapping[8] = &unknown_digits[i];
            break;
        case 3: // SEVEN
            digit_mapping[7] = &unknown_digits[i];
            break;
        case 4: // FOUR
            digit_mapping[4] = &unknown_digits[i];
            break;
        case 2: // ONE
            digit_mapping[1] = &unknown_digits[i];
            break;
        }
    }

    // DIGITS 7 3 4 2 ARE KNOWN

    // TOP PART BY XORING 7 AND 1
    seven_segment.top = position_value(digit_mapping[7]->value ^ digit_mapping[1]->value);

    // BOTTOM PART BY XORING 9 (UNKNOWN, SEARCH) WITH THE UNION OF 7 AND 4
    for (size_t i = 0; i < DIGIT_LEN; i++)
    {
        u_int8_t union_7_4 = digit_mapping[7]->value | digit_mapping[4]->value;
        if (unknown_digits[i].enabled == 6 && contains(unknown_digits[i].value, union_7_4))
        {
            seven_segment.bottom = position_value(~union_7_4 & unknown_digits[i].value);
            digit_mapping[9] = &unknown_digits[i];
            break;
        }
    }

    // MIDDLE PART BY XORING 3 (UNKNOWN, SEARCH) WITH THE UNION OF 1 AND TOP/BOTTOM SEGMENTS
    for (size_t i = 0; i < DIGIT_LEN; i++)
    {
        u_int8_t union_b_7 = CHAR_BIN(seven_segment.bottom) | digit_mapping[7]->value;
        if (unknown_digits[i].enabled == 5 && contains(unknown_digits[i].value, union_b_7))
        {
            seven_segment.middle = position_value(~union_b_7 & unknown_digits[i].value);
            break;
        }
    }

    // TOP LEFT FROM 9 XOR WITH 3
    digit_mapping[3] = new_digit(digit_mapping[7]->value | CHAR_BIN(seven_segment.middle) | CHAR_BIN(seven_segment.bottom)); // SET 3
    seven_segment.left_top = position_value(digit_mapping[9]->value ^ digit_mapping[3]->value);
    // BOTTOM LEFT FROM 5 XOR WITH SEGMENTS TOP, MIDDLE, BOTTOM, TOP LEFT
    for (size_t i = 0; i < DIGIT_LEN; i++)
    {
        u_int8_t union_t_b_m_lt = CHAR_BIN(seven_segment.top) | CHAR_BIN(seven_segment.bottom) | CHAR_BIN(seven_segment.middle) | CHAR_BIN(seven_segment.left_top);
        if (unknown_digits[i].enabled == 5 && contains(unknown_digits[i].value, union_t_b_m_lt))
        {
            seven_segment.right_bottom = position_value(~union_t_b_m_lt & unknown_digits[i].value);
            digit_mapping[5] = &unknown_digits[i];
            break;
        }
    }

    // TOP RIGHT FROM ONE XOR BOTTOM RIGHT SEGMENT
    seven_segment.right_top = position_value(CHAR_BIN(seven_segment.right_bottom) ^ digit_mapping[1]->value);

    // BOTTOM LEFT FROM 8 XOR EVERY OTHER SEGMENT
    seven_segment.left_bottom = position_value(digit_mapping[8]->value ^ (digit_mapping[1]->value | digit_mapping[5]->value));

    // FINISH MAPPING WIRES TO SEGMENTS
    // CONSTRUCT REMAINING DIGITS
    // DIGITS nine, eight, seven, five, four, three, one ARE KNOWN

    digit_mapping[0] = new_digit(digit_mapping[8]->value ^ CHAR_BIN(seven_segment.middle));                                            // ZERO
    digit_mapping[2] = new_digit(digit_mapping[8]->value ^ (CHAR_BIN(seven_segment.left_top) | CHAR_BIN(seven_segment.right_bottom))); // TWO
    digit_mapping[6] = new_digit(digit_mapping[5]->value | CHAR_BIN(seven_segment.left_bottom));                                       // SIX
}

unsigned int entry_to_number(DIGIT **digit_mapping, char **numbers)
{
    unsigned int number = 0;
    unsigned int shift = 1;
    for (int i = (SEGMENT_GROUP_LEN - 1); i >= 0; i--)
    {
        number += shift * get_digit_value(digit_mapping, str_to_digit(numbers[i]));
        shift *= 10;
    }
    return number;
}

// ---
// RELATIONSHIP datatype

typedef struct
{
    char **input;
    char **output;
} RELATIONSHIP;

RELATIONSHIP *new_relationship()
{
    RELATIONSHIP *p_relationship = calloc(1, sizeof(RELATIONSHIP));
    p_relationship->input = calloc(INPUT_COUNT, sizeof(char *));
    p_relationship->output = calloc(OUTPUT_COUNT, sizeof(char *));
    for (size_t i = 0; i < INPUT_COUNT; i++)
    {
        p_relationship->input[i] = calloc(CABLES, sizeof(char));
    }
    for (size_t i = 0; i < OUTPUT_COUNT; i++)
    {
        p_relationship->output[i] = calloc(CABLES, sizeof(char));
    }
    return p_relationship;
}

RELATIONSHIP *string_2_relationship(char *string)
{
    RELATIONSHIP *p_relationship = new_relationship();
    char input_buf[BUFSIZ];
    char output_buf[BUFSIZ];
    sscanf(string, "%[^|] | %[^\n]\n", input_buf, output_buf);
    char *wire = strtok(input_buf, " ");
    for (size_t i = 0; wire != NULL; i++)
    {
        p_relationship->input[i] = strdup(wire);
        wire = strtok(NULL, " ");
    }
    wire = strtok(output_buf, " ");
    for (size_t i = 0; wire != NULL; i++)
    {
        p_relationship->output[i] = strdup(wire);
        wire = strtok(NULL, " ");
    }
    return p_relationship;
}

//

RELATIONSHIP **read_input(FILE *p_file)
{
    char buffer[BUFSIZ];
    RELATIONSHIP **p_wire_relationships = calloc(ENTRIES, sizeof(RELATIONSHIP *));

    for (size_t i = 0; fgets(buffer, sizeof buffer, p_file) > 0; i++)
    {
        p_wire_relationships[i] = string_2_relationship(buffer);
    }
    return p_wire_relationships;
}

int main(int argc, char const *argv[])
{
    FILE *p_file = fopen("input", "r");
    RELATIONSHIP **p_wire_relationships = read_input(p_file); // I realize now that we could do this line-by-line actualy and save memory.

    unsigned int total = 0;
    for (size_t i = 0; i < ENTRIES; i++)
    {
        DIGIT **digit_mapping = calloc(DIGIT_LEN, sizeof(DIGIT *));
        set_digit_mapping(p_wire_relationships[i]->input, digit_mapping);
        unsigned int number = entry_to_number(digit_mapping, p_wire_relationships[i]->output);
        printf("%d\n", number);
        total += number;
        free(digit_mapping);
    }

    printf("%d\n", total);
    return 0;
}

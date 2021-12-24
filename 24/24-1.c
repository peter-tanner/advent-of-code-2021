#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define PROGRAM_SIZE 512

typedef enum
{
    INP = 0,
    ADD,
    MUL,
    DIV,
    MOD,
    EQL,

    SUB,
} OPCODE;

const OPCODE INVERSE[] = {
    INP,
    SUB,
    DIV,
    MUL,
    MOD, //
    EQL,
};

const char *OPCODE_STRINGS[] = {
    "inp",
    "add",
    "mul",
    "div",
    "mod",
    "eql",
};

OPCODE str2opcode(char *str)
{
    int i = 0;
    while (strcmp(str, OPCODE_STRINGS[i]) != 0)
        i++;
    return i;
}

typedef long int NUMBER;

typedef enum
{
    UNDEFINED = 0,
    LITERAL,
    VARIABLE,
} VALUE_TYPE;

typedef struct
{
    NUMBER value;
    VALUE_TYPE type;
} VALUE;

typedef struct
{
    OPCODE opcode;
    VALUE v1;
    VALUE v2;
} INSTRUCTION;

void print_instruction(INSTRUCTION instruction)
{
    printf("%s\t|", OPCODE_STRINGS[instruction.opcode]);
    if (instruction.v1.type == VARIABLE)
        putchar(instruction.v1.value + 'w');
    else
        printf("%li", instruction.v1.value);
    putchar('\t');
    if (instruction.v2.type == VARIABLE)
        putchar(instruction.v2.value + 'w');
    else
        printf("%li", instruction.v2.value);
    putchar('\n');
}

#define CRASH(condition)                   \
    if (condition)                         \
    {                                      \
        printf("CRASH AT %d\n", __LINE__); \
        exit(EXIT_FAILURE);                \
    }

NUMBER run_program(INSTRUCTION *program, NUMBER program_size, NUMBER *rom)
{

    NUMBER rom_ptr = 0;
    NUMBER registers[4] = {0};

#define GET_VALUE(instruction, registers) (instruction.v2.type == VARIABLE ? registers[instruction.v2.value] : instruction.v2.value)
    for (NUMBER i = 0; i < program_size; i++)
    {
        INSTRUCTION instruction = program[i];
        CRASH(instruction.v1.type == UNDEFINED);
        CRASH(instruction.v1.type != VARIABLE);
        switch (instruction.opcode)
        {
        case INP:
            registers[instruction.v1.value] = rom[rom_ptr];
            rom_ptr++;
            break;
        case ADD:
            CRASH(instruction.v2.type == UNDEFINED);
            registers[instruction.v1.value] += GET_VALUE(instruction, registers);
            break;
        case MUL:
            CRASH(instruction.v2.type == UNDEFINED);
            registers[instruction.v1.value] *= GET_VALUE(instruction, registers);
            break;
        case DIV:
            CRASH(instruction.v2.type == UNDEFINED);
            CRASH(instruction.v2.value == 0);
            registers[instruction.v1.value] /= GET_VALUE(instruction, registers);
            break;
        case MOD:
            CRASH(instruction.v2.type == UNDEFINED);
            CRASH(instruction.v2.value <= 0);
            registers[instruction.v1.value] %= GET_VALUE(instruction, registers);
            break;
        case EQL:
            CRASH(instruction.v2.type == UNDEFINED);
            registers[instruction.v1.value] = (registers[instruction.v1.value] == GET_VALUE(instruction, registers));
            break;
        default:
            break;
        }
    }
#undef GET_VALUE
    return registers[3];
}

int main(int argc, char const *argv[])
{
    FILE *p_file = fopen("input", "r");
    char buf[BUFSIZ];
    INSTRUCTION program[PROGRAM_SIZE];
    NUMBER program_size = 0;

#define STR2VALUE(token, instruction, val)                                                    \
    token = strtok(NULL, " ");                                                                \
    if (token != NULL)                                                                        \
    {                                                                                         \
        (instruction).val.type = (isdigit(token[0]) || token[0] == '-') ? LITERAL : VARIABLE; \
        if ((instruction).val.type == VARIABLE)                                               \
            (instruction).val.value = token[0] - 'w';                                         \
        else                                                                                  \
            (instruction).val.value = strtol(token, NULL, 0);                                 \
    }

    while (fgets(buf, sizeof buf, p_file) != NULL)
    {
        // READ INSTRUCTION
        buf[strcspn(buf, "\n")] = '\0'; // DELETE TRAILING \n

        char *token = strtok(buf, " ");
        program[program_size].opcode = str2opcode(token);
        STR2VALUE(token, program[program_size], v1);
        STR2VALUE(token, program[program_size], v2);
#undef STR2VALUE
        program_size++;
    }

    return 0;
}

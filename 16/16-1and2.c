#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// SAMPLE INPUT
#define INPUT "9C0141080250320F1802104A08"

#define TYPE_LITERAL(t) ((t) == 4)

#define HEADER_LEN 6
#define BIT_TYPE_LEN 15
#define PACKET_TYPE_LEN 11
#define BLOCK_LEN 5
#define DIGIT_LEN 4

typedef long int INTEGER;

char *BIN_MAP[] = {
    "0000",
    "0001",
    "0010",
    "0011",
    "0100",
    "0101",
    "0110",
    "0111",
    "1000",
    "1001",
    "1010",
    "1011",
    "1100",
    "1101",
    "1110",
    "1111",
};

typedef struct
{
    int packets;
    int bits;
    INTEGER value;
    int version_total;
} COUNTER;

char *hex2bin(char *hex)
{
    char *bin = calloc(strlen(hex) * DIGIT_LEN, sizeof(char));
    char *bin_ = bin;
    for (char *c = hex; *c != '\0'; c++)
    {
        if (*c >= '0' && *c <= '9')
            strcpy(bin_, BIN_MAP[*c - '0']);
        else
            strcpy(bin_, BIN_MAP[*c - 'A' + 10]);
        bin_ += DIGIT_LEN;
    }
    return bin;
}

COUNTER *read_literal(char *bin_literal)
{
    COUNTER *ctr = calloc(1, sizeof(COUNTER));
    char value_buf[BUFSIZ] = {0};
    bool end_of_block = false;
    for (int i = 0;; i++)
    {
        memcpy(&value_buf[i * DIGIT_LEN], &bin_literal[ctr->bits + 1], DIGIT_LEN);
        if (bin_literal[ctr->bits] == '0')
            end_of_block = true;
        ctr->bits += BLOCK_LEN;
        if (end_of_block)
            break;
    }
    ctr->value = strtol(value_buf, NULL, 2);
    return ctr;
}

COUNTER *read_header(char *bin)
{
    COUNTER *ctr = calloc(1, sizeof(COUNTER));
    ctr->bits = HEADER_LEN;
    INTEGER values[BUFSIZ] = {0};
    int value_count = 0;
    char buf[3];
    memcpy(buf, &bin[0], 3);
    int version = strtol(buf, NULL, 2);
    memcpy(buf, &bin[3], 3);
    int type = strtol(buf, NULL, 2);

    ctr->version_total += version;

    if (TYPE_LITERAL(type))
    {
        COUNTER *ctr_ = read_literal(bin + HEADER_LEN);
        ctr->bits += ctr_->bits;
        ctr->value = ctr_->value;
        ctr->packets++;
        free(ctr_);
        return ctr;
    }

    ctr->packets++;

    // READ PACKETS
    if (bin[6] == '0') // 15 BIT NUMBER
    {
        char value_buf[BIT_TYPE_LEN];
        ctr->bits += BIT_TYPE_LEN + 1;
        memcpy(value_buf, &bin[HEADER_LEN + 1], BIT_TYPE_LEN);
        int max_bits = (int)strtol(value_buf, NULL, 2);
        // printf("TOTAL_LEN %d %s\n", max_bits, value_buf);
        bin += HEADER_LEN + BIT_TYPE_LEN + 1;
        int bits = 0;
        while (bits < max_bits)
        {
            COUNTER *ctr_ = read_header(bin);
            ctr->bits += ctr_->bits;
            bin += ctr_->bits;
            bits += ctr_->bits;
            ctr->version_total += ctr_->version_total;
            values[value_count] = ctr_->value;
            value_count++;
            free(ctr_);
        }
    }
    else
    { // 11 BIT NUMBER
        char value_buf[PACKET_TYPE_LEN];
        ctr->bits += PACKET_TYPE_LEN + 1;
        memcpy(value_buf, &bin[HEADER_LEN + 1], PACKET_TYPE_LEN);
        int max_packets = (int)strtol(value_buf, NULL, 2);
        // printf("TOTAL_PACKETS %d %s\n", max_packets, value_buf);
        int count = 0;
        bin += HEADER_LEN + PACKET_TYPE_LEN + 1;
        while (count < max_packets)
        {
            COUNTER *ctr_ = read_header(bin);
            ctr->bits += ctr_->bits;
            bin += ctr_->bits;
            count += ctr_->packets;
            ctr->version_total += ctr_->version_total;
            values[value_count] = ctr_->value;
            value_count++;
            free(ctr_);
        }
    }
    switch (type)
    {
    case 0: // SUM
        for (int i = 0; i < value_count; i++)
            ctr->value += values[i];
        break;
    case 1: // PROD
        ctr->value = values[0];
        for (int i = 1; i < value_count; i++)
            ctr->value *= values[i];
        break;
    case 2: // MIN
        ctr->value = values[0];
        for (int i = 1; i < value_count; i++)
            if (values[i] < ctr->value)
                ctr->value = values[i];
        break;
    case 3: // MAX
        ctr->value = values[0];
        for (int i = 1; i < value_count; i++)
            if (values[i] > ctr->value)
                ctr->value = values[i];
        break;
    case 5: // GT
        ctr->value = values[0] > values[1];
        break;
    case 6: // LT
        ctr->value = values[0] < values[1];
        break;
    case 7: // EQ
        ctr->value = values[0] == values[1];
        break;
    default:
        printf("ERROR\n");
    }
    return ctr;
}

int main(int argc, char const *argv[])
{
    char hex_string[] = INPUT "\0";
    char *bin = hex2bin(hex_string);
    COUNTER *ctr = read_header(bin);
    printf("VERSION_TOTAL %d VALUE %li\n", ctr->version_total, ctr->value);
    return 0;
}

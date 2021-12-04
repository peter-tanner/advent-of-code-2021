#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BOARD_SIZE 5
#define BOARD_BUF_SIZ 100

typedef struct
{
    bool **state;
    unsigned int **value;
    unsigned int *column_done;
    unsigned int *row_done;
    bool won;
} BINGO_BOARD;

BINGO_BOARD *new_bingo_board()
{
    BINGO_BOARD *p_board = calloc(1, sizeof(BINGO_BOARD));
    p_board->column_done = calloc(BOARD_SIZE, sizeof(unsigned int));
    p_board->row_done = calloc(BOARD_SIZE, sizeof(unsigned int));
    p_board->won = false;

    p_board->state = calloc(BOARD_SIZE, sizeof(bool *));
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        p_board->state[i] = calloc(BOARD_SIZE, sizeof(bool));
    }
    p_board->value = calloc(BOARD_SIZE, sizeof(unsigned int *));
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        p_board->value[i] = calloc(BOARD_SIZE, sizeof(unsigned int));
    }
    return p_board;
}

void print_board(BINGO_BOARD *board)
{
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        for (size_t j = 0; j < BOARD_SIZE; j++)
        {
            printf("%i(%i)\t", board->value[i][j], board->state[i][j]);
        }
        putchar('\n');
    }
}

void *fill_bingo_board_row(BINGO_BOARD *board, char *line, char row)
{
    char *num_str = strtok(line, " ");
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        board->value[row][i] = strtoul(num_str, NULL, 10);
        num_str = strtok(NULL, " ");
    }
}

//

BINGO_BOARD **read_cards(FILE *file, int *n_boards)
{
    char buf[BUFSIZ];
    BINGO_BOARD **boards = calloc(BOARD_BUF_SIZ, sizeof(BINGO_BOARD *));
    *n_boards = -1;
    size_t row = 0;
    while (fgets(buf, sizeof buf, file) != NULL)
    {
        if (buf[0] == '\n' || buf[0] == '\0')
        {
            row = 0;
            (*n_boards)++;
            boards[*n_boards] = new_bingo_board();
            // printf("New board\n");
        }
        else
        {
            fill_bingo_board_row(boards[*n_boards], buf, row);
            row++;
        }
    }
    (*n_boards)++;
    return boards;
}

unsigned int sum_unmarked_board(BINGO_BOARD *board)
{
    unsigned int sum = 0;
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        for (size_t j = 0; j < BOARD_SIZE; j++)
        {
            if (!board->state[i][j])
                sum += board->value[i][j];
        }
    }
    return sum;
}

unsigned int mark_board(BINGO_BOARD *board, unsigned int n)
{
    bool done = false;
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        for (size_t j = 0; j < BOARD_SIZE; j++)
        {
            if (board->value[i][j] == n && !board->state[i][j])
            {
                board->state[i][j] = true;
                board->row_done[i]++;
                board->column_done[j]++;
                if (board->row_done[i] >= BOARD_SIZE || board->column_done[j] >= BOARD_SIZE)
                {
                    done = true;
                }
            }
        }
    }
    if (done)
    {
        board->won = true;
        // print_board(board);
        return sum_unmarked_board(board);
    }
}

void mark_boards(BINGO_BOARD **boards, int n_boards, char *nums_str, unsigned int *answer)
{
    char *num_str = strtok(nums_str, ",");
    while (num_str != NULL)
    {
        unsigned int number = strtoul(num_str, NULL, 10);
        unsigned int last_sum = 0, n_remaining_boards = 0;
        for (size_t i = 0; i < n_boards; i++)
        {
            if (!boards[i]->won)
            {
                last_sum = mark_board(boards[i], number);
                n_remaining_boards++;
            }
        }
        if (n_remaining_boards == 1) //Imply last board
        {
            *answer = number * last_sum;
        }

        num_str = strtok(NULL, ",");
    }
}

int main(int argc, char const *argv[])
{
    FILE *p_file = fopen("input", "r");
    char buf[BUFSIZ];
    fgets(buf, sizeof buf, p_file);
    int n_boards;
    BINGO_BOARD **boards = read_cards(p_file, &n_boards);
    // for (size_t i = 0; i < n_boards; i++)
    // {
    //     printf("%d\n", i);
    //     print_board(boards[i]);
    // }

    unsigned int answer;

    mark_boards(boards, n_boards, buf, &answer);

    printf("%d\n", answer);
    exit(EXIT_SUCCESS);
    return 0;
}

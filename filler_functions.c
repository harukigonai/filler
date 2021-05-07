#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "filler_functions.h"

void print_board(int *board, int is_client) {
    for (int i = 0; i < 7 * 8; i++) {
        int index = i;
        if (is_client) 
            index = (7 * 8 - 1) - i;
        
        printf("\033[3%dm▇\033[0m ", board[index]);
        
        if (i % 8 == 7) 
            printf("\n");
    }
    printf("\n");
}

void make_move(int *board, int *tiles, int *tiles_size_ptr, int move) {    
    int new_tiles[7 * 8];
    memset(new_tiles, 0, sizeof(new_tiles));
    for (int i = 0; i < 7 * 8; i++) {
        if (tiles[i] == 1) {
            if (i % 8 != 0 && board[i - 1] == move) 
                new_tiles[i - 1] = 1;
            if (i % 8 != 7 && board[i + 1] == move) 
                new_tiles[i + 1] = 1;
            if (i / 8 != 0 && board[i - 8] == move)  
                new_tiles[i - 8] = 1;
            if (i / 8 != 6 && board[i + 8] == move)  
                new_tiles[i + 8] = 1;
        }
    }
    for (int i = 0; i < 7 * 8; i++) {
        if (tiles[i] == 1) 
            board[i] = move;
        if (new_tiles[i] == 1) { 
            tiles[i] = 1;
            (*tiles_size_ptr)++;
        }
    }
}

void move_prompt(int current_color, int opponent_color, int *choice_ptr) {
    char buf[4096];
    while (1) {
        printf("Pick color:\n");

        for (int i = 0; i < 6; i++) {
            if (i == current_color || i == opponent_color) 
                printf("     ");
            else 
                printf("%d:\033[3%dm▇\033[0m  ", i + 1, i);
        }
        printf("\n");
        
        scanf("%s", buf);
        if (strlen(buf) == 1) {
            *choice_ptr = atoi(buf) - 1;
            if (0 <= *choice_ptr && *choice_ptr <= 5 &&
                *choice_ptr != current_color && *choice_ptr != opponent_color) 
                break;
        }
    }
}

void print_move_chosen(int color_picked, int is_client) {
    if (is_client) 
        printf("Opponent ");
    else 
        printf("You ");
    printf("picked \033[3%dm▇\033[0m.\n", color_picked);
}

int num_gen(int exclude_1, int exclude_2) 
{
    int num;
    if (exclude_1 == exclude_2)
        return exclude_1 <= (num = random() % 5) ? num + 1 : num;
    else {
        int smaller_exclude;
        int larger_exclude;
        if (exclude_1 > exclude_2) {
            smaller_exclude = exclude_2;
            larger_exclude = exclude_1;
        }
        else {
            smaller_exclude = exclude_1;
            larger_exclude = exclude_2;
        }
        
        if ((num = random() % 4) < smaller_exclude && num < larger_exclude)
            return num;
        else if (smaller_exclude <= num && num + 1 < larger_exclude)
            return num + 1;
        else
            return num + 2;
    } 
}

void randomize_board(int *board) 
{
    srandom(time(NULL));
    board[0] = random() % 6;
    for (int col = 1; col < 8; col++) 
        board[col] = num_gen(board[col - 1], board[col - 1]);
    for (int row = 1; row < 7; row++) {
        board[row * 8] = num_gen(board[(row - 1) * 8], board[(row - 1) * 8]);
        for (int col = 1; col < 8; col++) 
            board[row * 8 + col] = num_gen(board[(row - 1) * 8 + col], 
                                           board[row * 8 + col - 1]);
    }
    
    if (board[6 * 8] == board[7])
        board[6 * 8] = num_gen(board[6 * 8 - 8], board[6 * 8 + 1]);
}

void reset_game(int *board, int *tiles, int *opp_tiles,
                int *tiles_size_ptr, int *opp_tiles_size_ptr)
{
    randomize_board(board);
    memset(tiles, 0, sizeof(int) * 7 * 8);
    memset(opp_tiles, 0, sizeof(int) * 7 * 8);
    tiles[6 * 8] = 1;
    opp_tiles[7] = 1;
    *tiles_size_ptr = 1;
    *opp_tiles_size_ptr = 1;
}

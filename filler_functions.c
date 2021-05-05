#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
            if (i % 8 != 0 && board[i - 1] == move && tiles[i - 1] == 0) 
                new_tiles[i - 1] = 1;
            if (i % 8 != 7 && board[i + 1] == move && tiles[i + 1] == 0) 
                new_tiles[i + 1] = 1;
            if (i / 8 != 0 && board[i - 8] == move && tiles[i - 8] == 0)  
                new_tiles[i - 8] = 1;
            if (i / 8 != 6 && board[i + 8] == move && tiles[i + 8] == 0)  
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
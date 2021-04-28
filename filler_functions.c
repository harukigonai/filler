#include <stdio.h>

void print_board(int *board) {
    for (int i = 0; i < 7 * 8; i++) {
        if (board[i] == 0)
            printf("\033[31m▇\033[0m ");
        else if (board[i] == 1)
            printf("\033[32m▇\033[0m ");
        else if (board[i] == 2)
            printf("\033[33m▇\033[0m ");
        else if (board[i] == 3)
            printf("\033[34m▇\033[0m ");
        else if (board[i] == 4)
            printf("\033[35m▇\033[0m ");
        else if (board[i] == 5)
            printf("\033[30m▇\033[0m ");
        
        if (i % 8 == 7) 
            printf("\n");
    }
}

void make_move(int *board, int *territory, int *territory_size, int move) {
    for (int i = 0; territory[i] != -1; i++) {
        board[territory[i]] = move;
        
        if (!(territory[i] % 8 == 0) && board[territory[i] - 1] == move)
            territory[(*territory_size)++] = i - 1;
        if (!(territory[i] % 8 == 7) && board[territory[i] - 8] == move)
            territory[(*territory_size)++] = i - 8;
        if (!(territory[i] / 8 == 0) && board[territory[i] + 1] == move)
            territory[(*territory_size)++] = i + 1;
        if (!(territory[i] / 8 == 6) && board[territory[i] + 8] == move)
            territory[(*territory_size)++] = i + 8;
    }
}
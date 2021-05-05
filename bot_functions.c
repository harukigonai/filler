#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <float.h>
#include "bot_functions.h"
#include "filler_functions.h"
#include "queue.h"

void init_board_state(struct Board_state *bdst_ptr, int *board, int *tiles, 
                      int tiles_size, int *opp_tiles, int opp_tiles_size)
{
    memcpy(&bdst_ptr->board, board, sizeof(bdst_ptr->board));
    memcpy(&bdst_ptr->tiles, tiles, sizeof(bdst_ptr->tiles));
    memcpy(&bdst_ptr->tiles_size, &tiles_size, sizeof(bdst_ptr->tiles_size));
    memcpy(&bdst_ptr->opp_tiles, opp_tiles, sizeof(bdst_ptr->opp_tiles));
    memcpy(&bdst_ptr->opp_tiles_size, &opp_tiles_size, 
           sizeof(bdst_ptr->opp_tiles_size));
}

int power(int base, int exponent)
{
    int output = 1;
    for (int i = 0; i < exponent; i++)
        output *= base;
    return output;
}

void minimax(double *tree, int position, int depth, int to_maximize)
{
    if (depth == 0) 
        return;
    
    if (to_maximize) {
        double max_eval = -DBL_MAX;
        for (int i = 0; i < 4; i++) {
            minimax(tree, position * 4 + 1 + i, depth - 1 , 0);
            max_eval = max_eval > tree[position * 4 + 1 + i] ? 
                       max_eval : tree[position * 4 + 1 + i];
        }
        tree[position] = max_eval;
    }
    else {
        double min_eval = DBL_MAX;
        for (int i = 0; i < 4; i++) {
            minimax(tree, position * 4 + 1 + i, depth - 1 , 1);
            min_eval = min_eval < tree[position * 4 + 1 + i] ? 
                       min_eval : tree[position * 4 + 1 + i];
        }
        tree[position] = min_eval;
    }
}

void get_move_bot(int *board, int *tiles, int *tiles_size_ptr, int *opp_tiles, 
                  int *opp_tiles_size_ptr, struct Queue *current, 
                  struct Queue *next, int layers, int *choice) 
{    
    struct Board_state *init_bdst_ptr = malloc(sizeof(struct Board_state));
    init_board_state(init_bdst_ptr, board, tiles, *tiles_size_ptr,
                     opp_tiles, *opp_tiles_size_ptr);
    enqueue(current, init_bdst_ptr);
    
    for (int i = 0; i < layers - 1; i++) {
        while(!queue_is_empty(current)) {
            struct Board_state *bdst_ptr = dequeue(current);
            for (int j = 0; j < 6; j++) {
                if (j != (bdst_ptr->board)[6 * 8] && j != (bdst_ptr->board)[7]) {
                    struct Board_state *new_bdst_ptr 
                        = malloc(sizeof(struct Board_state));
                    memcpy(new_bdst_ptr, bdst_ptr, sizeof(struct Board_state));
                    
                    if (i % 2 == 0) 
                        make_move(new_bdst_ptr->board, new_bdst_ptr->opp_tiles, 
                                  &(new_bdst_ptr->opp_tiles_size), j);
                    else 
                        make_move(new_bdst_ptr->board, new_bdst_ptr->tiles, 
                                  &(new_bdst_ptr->tiles_size), j);
                    enqueue(next, new_bdst_ptr);
                }
            }
            free(bdst_ptr);
        }        
        struct Queue *temp_queue_ptr = current;
        current = next;
        next = temp_queue_ptr;
    }
    
    int tree_size = (1 - power(4, layers)) / (1 - 4);
    double tree[tree_size];
    
    for (int i = tree_size - power(4, layers - 1); i < tree_size; i++) {
        struct Board_state *bdst_ptr = dequeue(current);
        tree[i] = ((double) bdst_ptr->opp_tiles_size) / 
                  (bdst_ptr->opp_tiles_size + bdst_ptr->tiles_size);  
        free(bdst_ptr);
    }    
    delete_queue(current);
    delete_queue(next);
    
    minimax(tree, 0, layers - 1, 1);
    int i;
    int j = 1;
    for (i = 0; i < 6; i++) {
        if (i != board[6 * 8] && i != board[7]) {
            if (tree[0] == tree[j]) {
                break;
            } 
            j++;
        }
    }
    *choice = i;    
} 

void player_vs_bot(int *board, int *tiles, int *tiles_size_ptr, 
                   int *opp_tiles, int *opp_tiles_size_ptr) 
{
    int choice;
    
    struct Queue current;
    init_queue(&current);
    struct Queue next;
    init_queue(&next);
    print_board(board, 0);
    while (1) {
        get_move_bot(board, tiles, tiles_size_ptr, opp_tiles, 
                     opp_tiles_size_ptr, &current, &next, 3, &choice);
        make_move(board, opp_tiles, opp_tiles_size_ptr, choice);
        print_move_chosen(choice, 1);
        
        if (*tiles_size_ptr + *opp_tiles_size_ptr == 7 * 8)
            break;
        
        print_board(board, 0);
        
        printf("\n%d %d\n", *tiles_size_ptr, *opp_tiles_size_ptr);
        printf("\nSizes : %d %d\n", *tiles_size_ptr, *opp_tiles_size_ptr);
        printf("\nTiles : ");
        for (int i = 0; i < 7 * 8; i++) {
            if (tiles[i] != 0) 
                printf(" %d ", i);
        }
        printf("\nOpp_Tiles : ");
        for (int i = 0; i < 7 * 8; i++) { 
            if (opp_tiles[i] != 0) 
                printf(" %d ", i);
        }
        printf("\n");
        
        move_prompt(board[6 * 8], board[7], &choice);
        print_move_chosen(choice, 0);
        make_move(board, tiles, tiles_size_ptr, choice);
        print_board(board, 0);
        
        printf("\n%d %d\n", *tiles_size_ptr, *opp_tiles_size_ptr);
        printf("\nSizes : %d %d\n", *tiles_size_ptr, *opp_tiles_size_ptr);
        printf("\nTiles : ");
        for (int i = 0; i < 7 * 8; i++) {
            if (tiles[i] != 0) 
                printf(" %d ", i);
        }
        printf("\nOpp_Tiles : ");
        for (int i = 0; i < 7 * 8; i++) { 
            if (opp_tiles[i] != 0) 
                printf(" %d ", i);
        }
        printf("\n");
        
        if (*tiles_size_ptr + *opp_tiles_size_ptr == 7 * 8)
            break;
    }
}
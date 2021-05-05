#ifndef __FILLER_FUNCTIONS_H__
#define __FILLER_FUNCTIONS_H__

void print_board(int *board, int is_client);
void add_tile(int *new_tiles, int *tiles_size_ptr, int tile);
void make_move(int *board, int *territory, int *territory_size, int move);
void move_prompt(int current_color, int opponent_color, int *choice);
void print_move_chosen(int color_picked, int is_opponent);
    
#endif
#ifndef __FILLER_FUNCTIONS_H__
#define __FILLER_FUNCTIONS_H__

void print_board(int *board, int is_client);
void make_move(int *board, int *tiles, int *tiles_size_ptr, int move);
void move_prompt(int current_color, int opponent_color, int *choice_ptr);
void print_move_chosen(int color_picked, int is_client);
int num_gen(int exclude_1, int exclude_2) ;
void randomize_board(int *board) ;
void reset_game(int *board, int *tiles, int *opp_tiles,
                int *tiles_size_ptr, int *opp_tiles_size_ptr);

#endif
#ifndef __FILLER_SERVER_FUNCTIONS_H__
#define __FILLER_SERVER_FUNCTIONS_H__
#include "queue.h"

struct Board_state {
    int board[7 * 8];
    int tiles[7 * 8];
    int tiles_size;
    int opp_tiles[7 * 8];
    int opp_tiles_size;
};

void init_board_state(struct Board_state *board_state_ptr, int *board, 
                      int *tiles, int tiles_size, int *opp_tiles, 
                      int opp_tiles_size);

void get_move_bot(int *board, int *tiles, int *tiles_size_ptr, int *opp_tiles, 
                  int *opp_tiles_size_ptr, struct Queue *current, struct Queue *next,
                  int layers, int *choice);

void player_vs_bot(int *board, int *tiles, int *tiles_size_ptr, 
                   int *opp_tiles, int *opp_tiles_size_ptr);

#endif
#ifndef __SERVER_FUNCTIONS_H__
#define __SERVER_FUNCTIONS_H__
void die(const char *message);
int socket_bind_listen(unsigned short port);
void play_game_with_player(int clntsock, int *board, 
                           int *tiles, int *tiles_size_ptr, 
                           int *opp_tiles, int *opp_tiles_size_ptr);
void player_vs_player(int *board, int *tiles, int *tiles_size_ptr, 
                      int *opp_tiles, int *opp_tiles_size_ptr);
#endif
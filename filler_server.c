#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server_functions.h"
#include "bot_functions.h"

int main(int argc, char **argv) 
{    
    int board[7 * 8];
    int tiles[7 * 8];
    int opp_tiles[7 * 8];
    int tiles_size, opp_tiles_size;
    
    char buf[4096];
    char mode;
    while (1) {
        printf("Type 0 for Player vs. Bot\nType 1 for Player vs. Player\n");
        scanf("%s", buf);
        if (strlen(buf) == 1) {
            mode = atoi(buf);
            if (0 <= mode && mode <= 1) 
                break;
        }
    }
    if (mode) 
        player_vs_player(board, tiles, &tiles_size, opp_tiles, &opp_tiles_size);
    else 
        player_vs_bot(board, tiles, &tiles_size, opp_tiles, &opp_tiles_size);
}
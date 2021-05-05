#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "filler_functions.h"
#include "bot_functions.h"

void die(const char *message) { perror(message); exit(1); }

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
}

void play_game_with_player(int clntsock, int *board, 
                           int *tiles, int *tiles_size_ptr, 
                           int *opp_tiles, int *opp_tiles_size_ptr) 
{
    int choice;
    
    print_board(board, 0);
    send(clntsock, board, sizeof(int) * 7 * 8, 0);
    printf("Waiting for client response . . .\n");
    while (recv(clntsock, &choice, sizeof(int), 0)) {
        make_move(board, opp_tiles, opp_tiles_size_ptr, ntohl(choice));
        print_move_chosen(ntohl(choice), 1);
        send(clntsock, board, sizeof(int) * 7 * 8, 0);
        
        if (*tiles_size_ptr + *opp_tiles_size_ptr == 7 * 8)
            break;
        
        print_board(board, 0);
        move_prompt(board[6 * 8], board[7], &choice);
        print_move_chosen(choice, 0);
        make_move(board, tiles, tiles_size_ptr, choice);
        print_board(board, 0);
        
        if (*tiles_size_ptr + *opp_tiles_size_ptr == 7 * 8)
            break;
        
        send(clntsock, board, sizeof(int) * 7 * 8, 0);
        printf("Waiting for client response . . .\n");
    }
}

void player_vs_player(int *board, int *tiles, int *tiles_size_ptr, 
                      int *opp_tiles, int *opp_tiles_size_ptr) 
{
    printf("What is the port on which you would like to host the server?\n");
    unsigned short port;
    scanf("%hu", &port);
    
    int servsock;
    if ((servsock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        die("socket failed");
    
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if (bind(servsock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        die("bind failed");

    if (listen(servsock, 5) < 0)
        die("listen failed");

    int clntsock;
    socklen_t clntlen;
    struct sockaddr_in clntaddr;

    while (1) {
        printf("Waiting for client connection . . .\n");
        
        clntlen = sizeof(clntaddr);
        if ((clntsock = accept(servsock,
                        (struct sockaddr *) &clntaddr, &clntlen)) < 0)
            die("accept failed");
        char *client_ip = inet_ntoa(clntaddr.sin_addr);
        printf("Connection started from: %s\n", client_ip);
        play_game_with_player(clntsock, board, tiles, tiles_size_ptr, 
                              opp_tiles, opp_tiles_size_ptr);
        fprintf(stderr, "Connection terminated from: %s\n", client_ip);
    }
}

int main(int argc, char **argv) 
{    
    int board[7 * 8]; 
    int tiles[7 * 8];
    int tiles_size = 1;
    int opp_tiles[7 * 8];
    int opp_tiles_size = 1;
    
    randomize_board(board);
    memset(tiles, 0, sizeof(tiles));
    memset(opp_tiles, 0, sizeof(opp_tiles));
    tiles[6 * 8] = 1;
    opp_tiles[7] = 1;
    
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
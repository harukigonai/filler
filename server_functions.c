#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "filler_functions.h"
#include "bot_functions.h"

void die(const char *message) { perror(message); exit(1); }

int socket_bind_listen(unsigned short port)
{
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
    
    return servsock;
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
        print_board(board, 0);
        send(clntsock, board, sizeof(int) * 7 * 8, 0);
        if (*tiles_size_ptr + *opp_tiles_size_ptr == 7 * 8)
            break;
        
        move_prompt(board[6 * 8], board[7], &choice);
        print_move_chosen(choice, 0);
        make_move(board, tiles, tiles_size_ptr, choice);
        print_board(board, 0);
        send(clntsock, board, sizeof(int) * 7 * 8, 0);
        
        if (*tiles_size_ptr + *opp_tiles_size_ptr == 7 * 8)
            break;
        
        printf("Waiting for client response . . .\n");
    }
    memset(board, -1, sizeof(int) * 7 * 8);
    send(clntsock, board, sizeof(int) * 7 * 8, 0);
    
    if (*tiles_size_ptr > *opp_tiles_size_ptr) {
        printf("Player wins!\n");
        board[0] = -1;
    }
    else if (*tiles_size_ptr < *opp_tiles_size_ptr) {
        printf("Opponent wins!\n");
        board[0] = 1;
    }
    else {
        printf("Tie!\n");
        board[0] = 0;
    }
    board[0] = htonl(board[0]);
    send(clntsock, &board[0], sizeof(int), 0);
}

void player_vs_player(int *board, int *tiles, int *tiles_size_ptr, 
                      int *opp_tiles, int *opp_tiles_size_ptr) 
{
    printf("What is the port on which you would like to host the server?\n");
    unsigned short port;
    scanf("%hu", &port);
    
    int servsock = socket_bind_listen(port);

    int clntsock;
    socklen_t clntlen;
    struct sockaddr_in clntaddr;

    while (1) {
        reset_game(board, tiles, opp_tiles, tiles_size_ptr, opp_tiles_size_ptr);
        
        printf("Waiting for client connection . . .\n");
        
        clntlen = sizeof(clntaddr);
        if ((clntsock = accept(servsock,
                        (struct sockaddr *) &clntaddr, &clntlen)) < 0)
            die("accept failed");
        char *client_ip = inet_ntoa(clntaddr.sin_addr);
        printf("Connection started from: %s\n", client_ip);
        play_game_with_player(clntsock, board, tiles, tiles_size_ptr, 
                              opp_tiles, opp_tiles_size_ptr);
        close(clntsock);
        fprintf(stderr, "Connection terminated from: %s\n", client_ip);
    }
}
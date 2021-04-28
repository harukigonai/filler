#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "filler_functions.h"

void die(const char *message) { perror(message); exit(1); }

void randomize_board(int *board) {
    for (int i = 0; i < 7 * 8; i++)
        board[i] = random() % 6;
}

void play_game_with_player(int clntsock, int *board, 
                           int *territory, int *territory_size, 
                           int *opp_territory, int *opp_territory_size) {
    send(clntsock, board, 7 * 8, 0);
    
    //determine who goes first
    int buf[4096];
//     buf[0] = random() * 2; 
//     recv(clntsock, buf, sizeof(char), 0);
    
//     if (buf[0])
//         printf("You ");
//     else
//         printf("Opponent ");
//     printf("goes first.\n");
    
    int still_playing = 1;
    int opp_choice;
    int my_choice;
    while (still_playing && recv(clntsock, buf, sizeof(int), 0)) {
        //print board
        opp_choice = buf[0];
        make_move(board, opp_territory, opp_territory_size, opp_choice);
        
        //print board
        printf("What is your move?\n");
        scanf("%d", &my_choice);
        make_move(board, territory, territory_size, my_choice);
    }
}

void player_vs_player(int *board, int *territory, int *territory_size, 
                      int *opp_territory, int *opp_territory_size) {
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

        play_game_with_player(clntsock, board, territory, territory_size, 
                              opp_territory, opp_territory_size);

        fprintf(stderr, "Connection terminated from: %s\n", client_ip);
    }
}

void player_vs_bot(int *board, int *territory, int *territory_size, 
                   int *opp_territory, int *opp_territory_size) {
    
}



int main() {
    srandom(time(NULL));
    
    printf("Type 0 for Player vs. Bot\nType 1 for Player vs. Player\n");
    int mode;
    scanf("%d", &mode);
    
    int board[7 * 8]; 
    int territory[7 * 8];
    int territory_size = 0;
    int opp_territory[7 * 8];
    int opp_territory_size = 0;
    memset(territory, -1, sizeof(territory));
        
    randomize_board(board);
    print_board(board);
    
    if (mode) 
        player_vs_player(board, territory, &territory_size, 
                         opp_territory, &opp_territory_size);
    else 
        player_vs_bot(board, territory, &territory_size, 
                      opp_territory, &opp_territory_size);
}
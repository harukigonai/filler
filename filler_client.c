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

static void die(const char *s) { perror(s); exit(1); }

int socket_connect(const char *ip, unsigned short port) 
{
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        die("socket failed");

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr)); 
    servaddr.sin_family      = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(ip);
    servaddr.sin_port        = htons(port); 

    if (connect(sock, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
        die("connect failed");
    
    return sock;
}

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s <server-ip> <server-port>\n",
                argv[0]);
        exit(1);
    }
    
    int sock = socket_connect(argv[1], atoi(argv[2]));

    int choice;
    int board[7 * 8];
    
    recv(sock, board, sizeof(int) * 7 * 8, MSG_WAITALL);
    print_board(board, 1);
    while (1) {
        move_prompt(board[6 * 8], board[7], &choice);
        print_move_chosen(choice, 0);
        choice = htonl(choice);
        send(sock, &choice, sizeof(int), 0);
        recv(sock, board, sizeof(int) * 7 * 8, MSG_WAITALL);
        if (board[0] == -1)
            break;
        print_board(board, 1);
        
        printf("Waiting for server response . . .\n");
        recv(sock, board, sizeof(int) * 7 * 8, MSG_WAITALL);
        if (board[0] == -1)
            break;
        print_move_chosen(board[6 * 8], 1);
        print_board(board, 1);
    }
    recv(sock, board, sizeof(int), MSG_WAITALL);
    board[0] = ntohl(board[0]);
    
    if (board[0] == 1) 
        printf("Player wins!\n");
    else if (board[0] == -1) 
        printf("Opponent wins!\n");
    else
        printf("Tie!\n");
    
    close(sock);
    return 0;
}
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

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s <server-ip> <server-port>\n",
                argv[0]);
        exit(1);
    }

    const char *ip = argv[1];
    unsigned short port = atoi(argv[2]);

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
        print_board(board, 1);
        
        printf("Waiting for client response . . .\n");
        recv(sock, board, sizeof(int) * 7 * 8, MSG_WAITALL);
        print_move_chosen(board[6 * 8], 1);
        print_board(board, 1);
    }

    close(sock);
    return 0;
}

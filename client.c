#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "grid.h"

#define PORT 8080

int main(){
    int sock = 0;
    struct sockaddr_in server_address;
    char buffer[1024] = {0};
    Grid grid;

    sock=socket(AF_INET, SOCK_STREAM, 0);
    if(sock <0){
        perror("socket creation error");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0){ //local
        perror("invalid addr");
        return -1;
    }
    if(connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) <0){
        perror("connect failed");
        return -1;
    }

    read(sock, &grid, sizeof(grid));
    display_grid(&grid);
    // printf("connected to server. type commands now:\n");

    while(!grid.game_over){
        printf("Enter command (w/a/s/d to move, q to quit): ");
        char command;
        scanf(" %c", &command);
        send(sock, &command, sizeof(command), 0);
        // memset(buffer, 0, 1024);
        // int bytes_read = read (sock, buffer, 1024);
        // if(bytes_read <= 0){
        //     break;

        //     printf("server: %s\n", buffer);
        // }

        read(sock, &grid, sizeof(grid));
        display_grid(&grid);
        close(sock);
        return 0;
    }
}
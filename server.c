#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "grid.h"

#define PORT 8080
#define MAX_PLAYERS 2

// void clienthandler(int client_socket){
//     char buffer[1024];
//     while(1){
//         memset(buffer, 0, 1024);
//         int bytes_read = read(client_socket, buffer, 1024);
//         if(bytes_read <= 0){
//             break;
//             printf("client: %s", buffer);
//             send(client_socket, buffer, bytes_read, 0);
//         }
//         close(client_socket);
//     }
// }

int main(){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    Grid grid;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == 0){
        perror("socket failed");
        exit(1); 
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0){
        perror("bind failed");
        exit(1);
    }
    if(listen(server_fd, MAX_PLAYERS) <0){
        perror("listen failed");
        exit(1);
    }
    printf("Waiting for players to join...\n");

    // printf("server started on port %d\n", 8080);
   for (int i = 0; i< MAX_PLAYERS; i++){
   
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if(new_socket < 0){
            perror("accept failed");
        }
        printf("Player %d connected\n", i+1);
        send(new_socket, &grid, sizeof(grid), 0);

        while(1){
            int valread = read(new_socket, buffer, 1024);
            if(valread == 0){
                printf("Player disconnected\n");
                break;
            }
            printf("Player %d command : %s\n", i+1, buffer);
        }
   }
   close(server_fd);
    return 0;

}
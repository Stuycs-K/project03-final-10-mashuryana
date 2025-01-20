#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "grid.h"

#define PORT 8080

void display_inventory(Grid *grid, int player_id){
    printf("Player %d's Inventory:\n", player_id);
    int hasItems = 0;
    for (int i = 0; i < MAX_INVENTORY_SIZE; i++){
        if (strlen(grid->inventories[player_id - 1][i].name) > 0){
            printf("- %s: %d (%s)\n", grid->inventories[player_id - 1][i].name, grid->inventories[player_id - 1][i].quantity, grid->inventories[player_id - 1][i].description);
            hasItems = 1;
        }
    }
    if (!hasItems){
        printf("Inventory is empty.\n");
    }
}

int main(){
    int sock = 0;
    struct sockaddr_in server_address;
    char command[2];
    Grid grid;
    int player_id = 0;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0){
        perror("Invalid address/ Address not supported");
        return -1;
    }

    if(connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection Failed");
        return -1;
    }

    while (1){
        int bytes_read = read(sock, &grid, sizeof(grid));
        if (bytes_read <= 0) {
            perror("Server disconnected or read error");
            break;
        }

        if (player_id == 0){
            for (int i = 0; i < MAX_PLAYERS; i++){
                if (grid.player_positions[i] != 0){
                    player_id = i + 1;
                    break;
                }
            }
        }

        display_grid(&grid);

        if(grid.game_over){
            printf("Game Over!\n");
            break;
        }

        printf("Enter command (w/a/s/d to move, i for inventory, g to get item, u to use potion, q to quit): ");

        if (fgets(command, sizeof(command), stdin) == NULL){
            perror("Error reading input");
            break;
        }

        command[strcspn(command, "\n")] = 0;

        if(strlen(command) == 0){
            continue;
        }

        if (strcmp(command, "i") == 0){
            display_inventory(&grid, player_id);
            continue; 
        } else if (strcmp(command, "q") == 0){
            send(sock, command, 1, 0);
            break;
        } else if(strchr("wasdgu", command[0]) != NULL && strlen(command) == 1) {
            send(sock, &command[0], 1, 0);
        } else{
            printf("Invalid command.\n");
        }
    }

    close(sock);
    return 0;
}
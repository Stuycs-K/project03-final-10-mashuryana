#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <time.h>
#include "grid.h"

#define PORT 8080
#define MAX_PLAYERS 2

typedef struct{
    int socket;
    int player_id;
} PlayerInfo;

PlayerInfo clients[MAX_PLAYERS] = {{0}, {0}};
Grid grid;

void move_monsters(Grid *grid){
    for (int i = 0; i < GRID_SIZE; i++){
        if (grid->grid[i] == 'M') {
            int moves[4] = {-5, -1, 1, 5};
            int random_move = rand() % 4;
            int new_pos = i + moves[random_move];

            if (new_pos >= 0 && new_pos < GRID_SIZE &&
                (i % 5 != 0 || random_move != 1) &&
                ((i + 1) % 5 != 0 || random_move != 2) &&
                grid->grid[new_pos] != 'M' && grid->grid[new_pos] != 'E'){
                grid->grid[i] = ' ';
                grid->grid[new_pos] = 'M';
            }
        }
    }
}

void *handle_client(void *arg){
    PlayerInfo *player = (PlayerInfo *)arg;
    int client_socket = player->socket;
    int player_id = player->player_id;
    char buffer[1024] = {0};
    int potion_used = 0;

    load_grid(&grid, "game_save.dat");

    for (int i = 0; i < MAX_PLAYERS; i++){
        if (grid.player_positions[i] == 0) {
            grid.player_positions[i] = i;
            break;
        }
    }

    send(client_socket, &grid, sizeof(Grid), 0);

    while (!grid.game_over){
        int valread = read(client_socket, buffer, 1024);
        if (valread <= 0){
            printf("Player %d disconnected\n", player_id);
            for (int i = 0; i < MAX_PLAYERS; i++) {
                if (grid.player_positions[i] == player_id - 1){
                    grid.player_positions[i] = 0;
                    break;
                }
            }
            break;
        }

        char command = buffer[0];
        int player_pos = grid.player_positions[player_id - 1];
        int prev_pos = player_pos;

        if (command == 'q') {
            grid.game_over = 1;
        } else {
            if (command == 'w' && player_pos >= 5){
                player_pos -= 5;
            } else if (command == 'a' && player_pos % 5 != 0){
                player_pos -= 1;
            } else if (command == 's' && player_pos < GRID_SIZE - 5){
                player_pos += 5;
            } else if (command == 'd' && (player_pos + 1) % 5 != 0){
                player_pos += 1;
            }
            grid.player_positions[player_id - 1] = player_pos;

            if (prev_pos != player_pos){
                char current_room = grid.grid[player_pos];
                if (current_room == 'T') {
                    if (!potion_used) {
                        grid.game_over = 1;
                        printf("Player %d triggered a trap!\n", player_id);
                    } else {
                        printf("Player %d avoided a trap using a potion!\n", player_id);
                        potion_used = 0;
                    }
                    grid.grid[player_pos] = 'X';
                } else if (current_room == '$'){
                    if (grid.treasure_count > 0) {
                        grid.treasure_count--;
                        grid.grid[player_pos] = ' ';
                        printf("Player %d found some treasure!\n", player_id);
                    } else {
                        printf("Player %d found an empty treasure room.\n", player_id);
                    }
                } else if (current_room == 'M'){
                    int monster_hp = grid.monster_hp[player_pos];
                    printf("Player %d encounters a monster (HP: %d)!\n", player_id, monster_hp);
                    while (monster_hp > 0) {
                        printf("Player %d attacks the monster!\n", player_id);
                        monster_hp -= 3;
                        if (monster_hp <= 0){
                            printf("Player %d defeated the monster!\n", player_id);
                            grid.grid[player_pos] = ' ';
                            grid.monster_hp[player_pos] = 0;
                            break;
                        }
                        if (!potion_used){
                            grid.game_over = 1;
                            printf("The monster attacks Player %d! You are slain!\n", player_id);
                            break;
                        } else {
                            printf("Player %d avoided the monster attack using a potion!\n", player_id);
                            potion_used = 0;
                            break;
                        }
                    }

                } else if (current_room == 'E'){
                    grid.game_over = 1;
                    printf("Player %d Escaped!\n", player_id);
                } else if (current_room == 'P'){
                    int found = 0;
                    for (int i = 0; i < MAX_INVENTORY_SIZE; i++){
                        if (strcmp(grid.inventories[player_id - 1][i].name, "Potion") == 0) {
                            grid.inventories[player_id - 1][i].quantity++;
                            found = 1;
                            break;
                        }
                    }
                    if (!found) {
                        strcpy(grid.inventories[player_id - 1][0].name, "Potion");
                        strcpy(grid.inventories[player_id - 1][0].description, "A healing potion.");
                        grid.inventories[player_id - 1][0].quantity++;
                    }
                    grid.grid[player_pos] = ' ';
                    printf("Player %d found a potion!\n", player_id);
                }

                if (grid.treasure_count == 0){
                    grid.game_over = 1;
                    printf("All treasure has been found! Game Over!\n");
                }
            }
        }
if (command == 'g'){
            for (int i = 0; i < MAX_ROOM_INVENTORY; i++){
                if (strlen(grid.room_inventories[player_pos][i].name) > 0) {
                    int found = 0;
                    for (int j = 0; j < MAX_INVENTORY_SIZE; j++){
                        if (strcmp(grid.inventories[player_id - 1][j].name, grid.room_inventories[player_pos][i].name) == 0) {
                            grid.inventories[player_id - 1][j].quantity++;
                            found = 1;
                            break;
                        }
                    }
                    if (!found){
                        strcpy(grid.inventories[player_id - 1][0].name, grid.room_inventories[player_pos][i].name);
                        strcpy(grid.inventories[player_id - 1][0].description, grid.room_inventories[player_pos][i].description);
                        grid.inventories[player_id - 1][0].quantity++;
                    }
                    strcpy(grid.room_inventories[player_pos][i].name, "");
                    strcpy(grid.room_inventories[player_pos][i].description, "");
                    grid.room_inventories[player_pos][i].quantity = 0;
                    printf("Player %d picked up a %s.\n", player_id, grid.inventories[player_id - 1][0].name);
                    break;
                }
            }
        }
        if (command == 'u') {
            for (int i = 0; i < MAX_INVENTORY_SIZE; i++) {
                if (strcmp(grid.inventories[player_id - 1][i].name, "Potion") == 0 && grid.inventories[player_id - 1][i].quantity > 0) {
                    grid.inventories[player_id - 1][i].quantity--;
                    potion_used = 1;
                    printf("Player %d used a potion!\n", player_id);
                    if (grid.inventories[player_id - 1][i].quantity <= 0) {
                        strcpy(grid.inventories[player_id - 1][i].name, "");
                        strcpy(grid.inventories[player_id - 1][i].description, "");
                    }
                    break;
                }
            }
        }
        move_monsters(&grid);

        for(int i = 0; i < MAX_PLAYERS; i++){
            if (clients[i].socket != 0) {
                send(clients[i].socket, &grid, sizeof(Grid), 0);
            }
        }
        save_grid(&grid, "game_save.dat");
    }

    close(client_socket);
    clients[player_id - 1].socket = 0;
    pthread_exit(NULL);
    return NULL;
}

int main(){
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pthread_t threads[MAX_PLAYERS];
    srand(time(NULL));

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, MAX_PLAYERS) < 0){
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for players to join...\n");

    int player_count = 0;
    while (player_count < MAX_PLAYERS){
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            continue;
    }

        printf("Player %d connected\n", player_count + 1);

        clients[player_count].socket = new_socket;
        clients[player_count].player_id = player_count + 1;

        if (pthread_create(&threads[player_count], NULL, handle_client, &clients[player_count]) != 0) {
            perror("pthread_create failed");
            
            close(new_socket);
            clients[player_count].socket = 0;
            continue;
        }
        player_count++;
    }

    for(int i = 0; i < MAX_PLAYERS; i++){
        if(threads[i] != 0) {
            pthread_join(threads[i], NULL);
        }
    }
    close(server_fd);
    return 0;
}
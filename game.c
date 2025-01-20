#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "grid.h"

void initialize_grid(Grid *grid){
    for (int i = 0; i < GRID_SIZE; i++){
        int random = rand() % 100;
        if (random < 10) {
            grid->grid[i] = 'T'; // Trap
        } else if (random < 20) {
            grid->grid[i] = '$'; // Treasure
        } else if (random < 30) {
            grid->grid[i] = 'M'; // Monster
        } else if (random < 35) {
            grid->grid[i] = 'P'; // Potion
        } else if (random < 40) {
            grid->grid[i] = 'E'; // Escape
        } else {
            grid->grid[i] = ' '; // Empty
        }
    }
    for (int i = 0; i < GRID_SIZE; i++){
        if (grid->grid[i] == 'M') {
            grid->monster_hp[i] = (rand() % 10) + 5; 
        } else {
            grid->monster_hp[i] = 0; 
        }
    }
    for (int i = 0; i < MAX_PLAYERS; i++){
        grid->player_positions[i] = 0;
        for (int j = 0; j < MAX_INVENTORY_SIZE; j++){
            strcpy(grid->inventories[i][j].name, "");
            strcpy(grid->inventories[i][j].description, "");
            grid->inventories[i][j].quantity = 0;
        }
    }
    for(int i = 0; i < GRID_SIZE; i++){
        for (int j = 0; j < MAX_ROOM_INVENTORY; j++){
            strcpy(grid->room_inventories[i][j].name, "");
            strcpy(grid->room_inventories[i][j].description, "");
            grid->room_inventories[i][j].quantity = 0;
        }
    }
    for (int i = 0; i < 5; i++){
        int room = rand() % GRID_SIZE;
        strcpy(grid->room_inventories[room][0].name, "Rock");
        strcpy(grid->room_inventories[room][0].description, "A small rock.");
        grid->room_inventories[room][0].quantity++;
    }

    grid->treasure_count = 5;
    grid->game_over = 0;
}

void display_grid(Grid *grid){
    printf("Current grid: \n");
    for (int i = 0; i < GRID_SIZE; i++){
        int player_here = 0;
        for (int j = 0; j < MAX_PLAYERS; j++){
            if (i == grid->player_positions[j]) {
                printf("[P%d] ", j + 1);
                player_here = 1;
                break;
            }
        }
        if(!player_here){
            printf("[%c] ", grid->grid[i]);
        }
        if((i + 1) % 5 == 0){
            printf("\n");
        }
    }
    printf("\n");
}

void save_grid(Grid *grid, const char *filename){
    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Error saving grid");
        return;
    }
    fwrite(grid, sizeof(Grid), 1, file);
    fclose(file);
}

void load_grid(Grid *grid, const char *filename){
    FILE *file = fopen(filename, "rb");
    if (!file) {
        initialize_grid(grid);
        save_grid(grid, filename);
        return;
    }
    fread(grid, sizeof(Grid), 1, file);
    fclose(file);
}
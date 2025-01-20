#ifndef GRID_H
#define GRID_H

#define GRID_SIZE 25
#define MAX_PLAYERS 2
#define MAX_INVENTORY_SIZE 5
#define MAX_ROOM_INVENTORY 3

typedef struct{
    char name[20];
    int quantity;
    char description[100];
} Item;

typedef struct{
    char grid[GRID_SIZE];
    int player_positions[MAX_PLAYERS];
    int treasure_count;
    int game_over;
    Item inventories[MAX_PLAYERS][MAX_INVENTORY_SIZE];
    Item room_inventories[GRID_SIZE][MAX_ROOM_INVENTORY];
    int monster_hp[GRID_SIZE]; 
} Grid;

void initialize_grid(Grid *grid);
void display_grid(Grid *grid);
void save_grid(Grid *grid, const char *filename);
void load_grid(Grid *grid, const char *filename);

#endif
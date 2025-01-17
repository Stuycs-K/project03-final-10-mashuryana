#ifndef GRID_H
#define GRID_H

#define GRID_SIZE 25

typedef struct{
    char grid[GRID_SIZE];
    int player_position;
    int treasure_count;
    int game_over;
} Grid;

void initialize_grid(Grid *gridNow);
void display_grid(Grid *grid);
void save_grid(Grid *grid, const char *filename);
void load_grid(Grid *grid, const char *filename);

#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <time.h>
#include <signal.h>
#include "grid.h";

void initialize_grid(Grid *grid){
    for (int i =0; i<GRID_SIZE; i++){
        int random = rand() % 100;
       if(random < 20){
        grid -> grid[i] = 'T'; // trap
       }
       else if (random < 40){
        grid -> grid[i] = '$'; //treasure
       }
       else if(random < 60){
        grid -> grid[i] = 'M'; // monster
       }
       else{
        grid -> grid[i] = ' '; // empty
       }
    }
    grid->player_position = 0;
    grid->treasure_count = 0;
    grid->game_over= 0;
    
}

void save_grid(Grid *grid, const char *filename){
    FILE *file = fopen(filename, "wb");
    if (!file){
        perror("error saving grid");
        return;
    }
    fwrite(grid, sizeof(Grid), 1, file);
    fclose(file);
}

void load_grid(Grid *grid, const char *filename){
    FILE *file = fopen(filename, "rb");
    if(!file){
        perror("error laoding grid");
        return;
    }
    fread(grid, sizeof(Grid), 1 , file);
    fclose(file);
}

void display_grid(Grid *grid){
    printf("Current grid: \n");
    for(int i = 0; i< GRID_SIZE; i++){
        if (i == grid->player_position){
            printf("[P] ");
        }
        else{
            printf("[%c] ", grid->grid[i]);
        }
        if((i+1) %5 == 0){
            printf("\n");
        }
    }
    printf("\n");
}

void handle_signals(int signal){
    if (signal == SIGUSR1){
        printf("You collected a treasure!\n");
    }
    else if(signal == SIGUSR2){
        printf("You trigged a trap!\n");
    }
}
int main(){
    signal(SIGUSR1, handle_signals);
    signal(SIGUSR2, handle_signals);

    int shmid;
    shmid = shmget (12412, sizeof(Grid), 0666 | IPC_CREAT); 
    if(shmid <0){
        perror("shmget fail");
        exit(1);
    }
    Grid *grid = (Grid *) shmat(shmid, NULL, 0);
    if(grid == (void *)-1){
        perror("shmat fail");
        exit(1);
    }
    initialize_grid(grid);
    display_grid(grid);

    while(!grid->game_over){
        char command;
        printf("Enter command (w/a/s/d to move, q to quit): ");
        scanf(" %c", &command);

        if(command == 'q'){
            grid->game_over = 1;
            printf("Game over! You quit the game.\n");
            break;
        }

        if(command == 'w' && grid->player_position >=5){
            grid->player_position -=5;
        }
        else if(command == 'a' && grid->player_position %5 !=0){
            grid->player_position -= 1;
        }
        else if(command == 's' && grid->player_position < GRID_SIZE -5){
            grid->player_position += 5;
        }
        else if(command == 'd' && (grid->player_position +1) %5 != 0){
            grid->player_position +=1;
        }

        char current_room = grid->grid[grid->player_position];
        if(current_room == 'T'){
            kill(getpid(), SIGUSR2); //trap
            grid->game_over = 1;
        }
        else if(current_room == '$'){
            grid->treasure_count++;
            kill(getpid(), SIGUSR1);
            grid->grid[grid->player_position] = ' ';
        }
        else if(current_room == 'M'){
            printf("You encountered a monster! Game over!\n");
            grid->game_over = 1;
        }

    

    display_grid(grid);
    save_grid(grid, "game_save.dat");
}
    shmdt(grid);
    return(0);

}

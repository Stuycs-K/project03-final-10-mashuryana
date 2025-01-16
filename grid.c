#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <time.h>
#include "grid.h";

typedef struct{
    char grid[25];
} Grid;

void initialize_grid(Grid *gridNow){
    for (int i =0; i<25; i++){
        int random = rand() % 100;
       if(random < 20){
        gridNow -> grid[i] = 'T'; // trap
       }
       else if (random < 30){
        gridNow -> grid[i] = '$'; //treasure
       }
       else if(random < 40){
        gridNow -> grid[i] = 'M'; // monster
       }
       else{
        gridNow -> grid[i] = ' '; // empty
       }
    }
    
}

int main(){
    int shmid;
    shmid = shmget (12412, sizeof(Grid), 0666 | IPC_CREAT); //0600?
    if(shmid <0){
        perror("shmget fail");
        exit(0);
    }
    Grid *grid = (Grid *) shmat(shmid, NULL, 0);
    if(grid == (void *)-1){
        perror("shmat fail");
        exit(0);
    }
    initialize_grid(grid);
    printf("grid initialized.");

    shmdt(grid);
    return(0);

}

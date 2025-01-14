#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/shm.h>
#include <sys/ipc.h>

typedef struct{
    char grid[25];
} Grid;

void initialize_grid(Grid *grid){
    for (int i =0; i<25; i++){
        ///? how to set up
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

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
    shmid = shmget (//key?????/)
}

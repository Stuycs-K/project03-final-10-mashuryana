#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <sys/ipc.h>

void lock(int semid){
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = 0;
    semop(semid, &sb, 1);
}

void unlock(int semid){
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = 1;
    sb.sem_flg = 0;
    semop(semid,&sb,1);
}

int main(){
    int semid = semget(//KEY, 1, 0666? | IPC_CREAT);
    semctl(semid, 0 , )
}
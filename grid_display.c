#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void clienthandler(int client_socket){
    char buffer[1024];
    while(1){
        memset(buffer, 0, 1024);
        int bytes_read = read(client_socket, buffer, 1024);
        if(bytes_read <= 0){
            break;
            printf("client: %s", buffer);
            send(client_socket, buffer, bytes_read, 0);
        }
        close(client_socket);
    }
}
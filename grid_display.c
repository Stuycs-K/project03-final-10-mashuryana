#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>


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

int main(){
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == 0){
        perror("socket failed");
        exit(0); 
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if(bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0){
        perror("bind failed");
        exit(0);
    }
    if(listen(server_fd, 2) <0){
        perror("listen failed");
        exit(0);
    }
    printf("server started on port %d\n", 8080);
    while(1){
        client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen); // or unsigned int
        if(client_socket < 0){
            perror("accept failed");
        }
        printf("new player connected\n");
        if (fork() == 0){
            close(server_fd);
            handle_client(client_socket);
            exit(0);
        }
        close(client_socket);
    }
    return 0;

}
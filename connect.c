#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){
    int sock = 0;
    struct sockaddr_in server_address;
    char buffer[1024];

    sock=socket(AF_INET, SOCK_STREAM, 0);
    if(sock <0){
        perror("socket creation error");
        return -1;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);

    if(inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0){ //local
        perror("invalid addr");
        return -1;
    }
}
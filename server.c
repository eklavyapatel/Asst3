#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "wtf.h"

int main(int argc, char* argv[])
{
    int socket_descriptor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    /* int socket(int domain, int type, int protocol)  */
    //have now receieved a socket descriptor
    
    struct sockaddr_in server_address;
    
    memset(&server_address, 0, sizeof(server_address));
    
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(1234);
    
    bind(socket_descriptor, (struct sockaddr*) &server_address, sizeof(server_address));
    
    listen(socket_descriptor, 20);
    
    struct sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);
    int client_socket = accept(socket_descriptor, (struct sockaddr*) &client_address, &client_address_size);
    
    char str[] = "Hello World!";
    write(client_socket, str, sizeof(str));
    
    close(client_socket);
    close(socket_descriptor);
    
    return 0;
}
